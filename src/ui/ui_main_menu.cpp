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
 * File: ui_main_menu.cpp
 * Created by kureii on 8/14/24
 */
#include <imgui.h>
#include <ui/ui_main_menu.h>

namespace linea_one::ui {

UiMainMenu::UiMainMenu(const std::shared_ptr<DocumentManager> &p_doc_man)
  : p_doc_man_(p_doc_man) {}

void UiMainMenu::Render() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New file", "Ctrl+N")) {
        NewFile();
      }
      if (ImGui::MenuItem("Open file")) {
        show_load_dialog_ = true;
      }
      if (ImGui::MenuItem("Save")) {

        if (p_doc_man_->GetCurrentDocumentIndex() >= 0 &&
          p_doc_man_->GetCurrentDocument()->path.empty()) {
          SaveFileDialog();
        } else {
          p_doc_man_->SaveDocument();
        }
      }
      if (ImGui::MenuItem("Save as")) {
        SaveFileDialog();
      }
      if (ImGui::MenuItem("Close file", "Ctrl+W")) {
        CloseFile();
      }
      if (ImGui::MenuItem("Exit")) {
        stop_rendering_ = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

bool UiMainMenu::IsShowUnsavedDialog() const { return show_unsaved_dialog_; }
bool UiMainMenu::IsShowSaveDialog() const { return show_save_dialog_; }

bool UiMainMenu::IsShowLoadDialog() const { return show_load_dialog_; }

bool UiMainMenu::IsStopRendering() const { return stop_rendering_; }

void UiMainMenu::SetShowUnsavedDialog(const bool show_unsaved_dialog) {
  show_unsaved_dialog_ = show_unsaved_dialog;
}

void UiMainMenu::SetShowSaveDialog(const bool show_save_dialog) {
  show_save_dialog_ = show_save_dialog;
}

void UiMainMenu::SetShowLoadDialog(const bool show_load_dialog) {
  show_load_dialog_ = show_load_dialog;
}

void UiMainMenu::NewFile() const { p_doc_man_->CreateNewDocument(); }

void UiMainMenu::CloseFile() {
  if (p_doc_man_->CloseDocumentWithCheck(
        p_doc_man_->GetCurrentDocumentIndex()) >= 0) {
    show_unsaved_dialog_ = true;
  }
}

void UiMainMenu::SaveFileDialog() {
  if (p_doc_man_->GetCurrentDocumentIndex() >= 0) {
    show_save_dialog_ = true;
  }
}

}  // namespace linea_one::ui