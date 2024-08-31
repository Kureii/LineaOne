/*
 * LineaOne - Specialized software for creating timelines for presentations.
 * Copyright (C) 2024 kureii
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * File: ui_modal_dialogs.cpp
 * Created by kureii on 8/15/24
 */
#include <imgui.h>
#include <ui/ui_modal_dialogs.h>
namespace linea_one::ui {

UiModalDialogs::UiModalDialogs(
  const std::shared_ptr<DocumentManager>& p_doc_man)
  : p_doc_man_(p_doc_man) {
#if defined(_WIN32) || defined(_WIN64)
  current_path_ = std::getenv("USERPROFILE");
#else
  current_path_ = std::getenv("HOME");
#endif
  RefreshDirectoryContents();
}

void UiModalDialogs::RenderUnsavedChanges() {
  show_unsaved_dialog_ = true;
  ImGui::OpenPopup("Unsaved Changes");

  if (ImGui::BeginPopupModal(
        "Unsaved Changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Document '%s' has unsaved changes. Do you want to save before "
                "closing?",
      p_doc_man_->GetSpecificDocument(p_doc_man_->GetDocToClose())
        .name.c_str());
    ImGui::Separator();

    if (ImGui::Button("Save", ImVec2(120, 0))) {
      if (p_doc_man_->GetSpecificDocument(p_doc_man_->GetDocToClose())
            .path.empty()) {
        show_save_dialog_ = true;
        show_unsaved_dialog_ = false;
      } else {
        p_doc_man_->SaveDocument();
        p_doc_man_->GetSpecificDocument(p_doc_man_->GetDocToClose()).saved =
          true;
        p_doc_man_->CloseDocument();
        ImGui::CloseCurrentPopup();
        show_unsaved_dialog_ = false;
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Don't Save", ImVec2(120, 0))) {
      p_doc_man_->CloseDocument();
      ImGui::CloseCurrentPopup();
      show_unsaved_dialog_ = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
      show_unsaved_dialog_ = false;
    }
    ImGui::EndPopup();
  }
}

void UiModalDialogs::RenderSaveDialog() {
  if (show_save_dialog_) {
    ImGui::OpenPopup("Save As");
  }

  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
  if (ImGui::BeginPopupModal(
        "Save As", &show_save_dialog_, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Current Path: %s", current_path_.string().c_str());

    if (ImGui::Button("..")) {
      if (current_path_.has_parent_path()) {
        current_path_ = current_path_.parent_path();
        RefreshDirectoryContents();
      }
    }

    // Přidáme oblast pro zobrazení seznamu souborů a složek
    ImGui::BeginChild("File Browser", ImVec2(0, 200), true);
    for (size_t i = 0; i < dir_contents_.size(); i++) {
      const auto& entry = dir_contents_[i];
      bool is_dir = is_directory(current_path_ / entry);
      std::string label = (is_dir ? "[D] " : "    ") + entry;

      if (ImGui::Selectable(
            label.c_str(), selected_index_ == static_cast<int>(i))) {
        selected_index_ = static_cast<int>(i);
        if (is_dir) {
          current_path_ /= entry;
          RefreshDirectoryContents();
          selected_index_ = -1;
        } else {
          strncpy(
            file_name_buffer_, entry.c_str(), sizeof(file_name_buffer_) - 1);
          file_name_buffer_[sizeof(file_name_buffer_) - 1] = '\0';
        }
      }
    }
    ImGui::EndChild();

    ImGui::InputText("File Name", file_name_buffer_, sizeof(file_name_buffer_));

    if (ImGui::Button("Save", ImVec2(120, 0))) {
      std::string file_name = file_name_buffer_;

      const std::string extension = ".jsonlo";
      if (file_name.length() >= extension.length()) {
        if (file_name.compare(file_name.length() - extension.length(),
              extension.length(), extension) != 0) {
          file_name += extension;
        }
      } else {
        file_name += extension;
      }

      auto full_path = current_path_ / file_name;
      p_doc_man_->GetCurrentDocument()->path = full_path;
      p_doc_man_->SaveDocument();
      show_save_dialog_ = false;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      show_save_dialog_ = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void UiModalDialogs::RenderLoadDialog() {
  if (show_load_dialog_) {
    ImGui::OpenPopup("Load File");
  }

  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
  if (ImGui::BeginPopupModal(
        "Load File", &show_load_dialog_, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Current Path: %s", current_path_.string().c_str());

    if (ImGui::Button("..")) {
      if (current_path_.has_parent_path()) {
        current_path_ = current_path_.parent_path();
        RefreshDirectoryContents();
      }
    }
    ImGui::BeginChild("File Browser", ImVec2(0, 300), true);
    for (size_t i = 0; i < dir_contents_.size(); i++) {
      const auto& entry = dir_contents_[i];
      std::filesystem::path entry_path = current_path_ / entry;
      bool is_dir = is_directory(entry_path);
      std::string label = (is_dir ? "[D] " : "    ") + entry;

      if (ImGui::Selectable(
            label.c_str(), selected_index_ == static_cast<int>(i))) {
        selected_index_ = static_cast<int>(i);
        if (is_dir) {
          current_path_ /= entry;
          RefreshDirectoryContents();
          selected_index_ = -1;
        } else if (entry_path.extension() == ".jsonlo") {
          selected_file_ = entry_path;
        }
      }
    }
    ImGui::EndChild();

    if (selected_file_) {
      ImGui::Text(
        "Selected File: %s", selected_file_->filename().string().c_str());

      if (ImGui::Button("Load", ImVec2(120, 0))) {
        p_doc_man_->LoadDocument(selected_file_.value());
        show_load_dialog_ = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
    }

    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      show_load_dialog_ = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void UiModalDialogs::RefreshDirectoryContents() {
  dir_contents_.clear();
  for (const auto& entry : std::filesystem::directory_iterator(current_path_)) {
    dir_contents_.push_back(entry.path().filename().string());
  }
  std::ranges::sort(dir_contents_);
}

void UiModalDialogs::SetShowUnsavedDialog(const bool show_unsaved_dialog) {
  show_unsaved_dialog_ = show_unsaved_dialog;
}

void UiModalDialogs::SetShowSaveDialog(const bool show_saved_dialog) {
  show_save_dialog_ = show_saved_dialog;
}

void UiModalDialogs::SetShowLoadDialog(const bool show_load_dialog) {
  show_load_dialog_ = show_load_dialog;
}

bool UiModalDialogs::GetShowUnsavedDialog() const {
  return show_unsaved_dialog_;
}

bool UiModalDialogs::GetShowSaveDialog() const { return show_save_dialog_; }

bool UiModalDialogs::GetShowLoadDialog() const { return show_load_dialog_; }
}  // namespace linea_one::ui
