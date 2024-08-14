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
 * File: ui_manager.cpp
 * Created by kureii on 8/14/24
 */
#include "ui_manager.h"

#include <SDL3/SDL_render.h>
#include <imgui.h>

namespace linea_one {

UiManager::UiManager(const std::shared_ptr<DocumentManager>& p_doc_man) : p_doc_man_(p_doc_man) {}

void UiManager::RenderMenu() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New file", "Ctrl+N")) {
        p_doc_man_->CreateNewDocument();
      }
      if (ImGui::MenuItem("Open file")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Save")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Save as")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Close file", "Ctrl+W")) {
        if (p_doc_man_->CloseDocumentWithCheck(p_doc_man_->GetCurrentDocumentIndex()) >= 0) {
          show_unsaved_dialog_ = true;
        }
      }
      if (ImGui::MenuItem("Exit")) {
        stop_rendering_ = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

void UiManager::RenderContent() {
  RenderTabs();
  if (const auto current_document = p_doc_man_->GetCurrentDocument()) {
    RenderTabContent(*current_document);
  }

  if (show_unsaved_dialog_) {
    RenderUnsavedChangesDialog();
  }
}

void UiManager::RenderTabs() {
  if (ImGui::BeginTabBar("DocumentTabs", ImGuiTabBarFlags_AutoSelectNewTabs)) {
    for (int32_t i = 0; i < p_doc_man_->DocumentSize(); ++i) {
      bool open = true;
      if (ImGui::BeginTabItem(p_doc_man_->GetSpecificDocument(i).name.c_str(), &open, ImGuiTabItemFlags_None)) {
        p_doc_man_->SetCurrentDocumentIndex(i);
        ImGui::EndTabItem();
      }
      if (!open) {
        const auto index_to_close = p_doc_man_->CloseDocumentWithCheck(i);
        if (index_to_close == -1) {
          i--;
        }
        if (index_to_close > -1) {
          show_unsaved_dialog_ = true;
        }
      }
    }
    if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
      p_doc_man_->CreateNewDocument();
    }
    ImGui::EndTabBar();
  }
}

void UiManager::RenderTabContent(Document& doc) {
  ImGui::Text("Content of document: %s", doc.name.c_str());
}

void UiManager::RenderUnsavedChangesDialog() {
  ImGui::OpenPopup("Unsaved Changes");

  if (ImGui::BeginPopupModal("Unsaved Changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Document '%s' has unsaved changes. Do you want to save before closing?",
                p_doc_man_->GetSpecificDocument(p_doc_man_->GetDocToClose()).name.c_str());
    ImGui::Separator();

    if (ImGui::Button("Save", ImVec2(120, 0))) {
      /* TODO save function */
      p_doc_man_->GetSpecificDocument(p_doc_man_->GetDocToClose()).saved = true;
      p_doc_man_->CloseDocument();
      ImGui::CloseCurrentPopup();
      show_unsaved_dialog_ = false;
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

void UiManager::SetShowUnsavedDialog(const bool show_unsaved_dialog) {
  show_unsaved_dialog_ = show_unsaved_dialog;
}

[[nodiscard]] bool UiManager::GetStopRendering() const {
  return stop_rendering_;
}

void UiManager::SetStopRendering(const bool stop_rendering) {
  stop_rendering_ = stop_rendering;
}
} // linea_one