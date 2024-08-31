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
#include <SDL3/SDL_render.h>
#include <imgui.h>
#include <ui/ui_manager.h>

namespace linea_one::ui {

UiManager::UiManager(const std::shared_ptr<DocumentManager>& p_doc_man,
    const std::shared_ptr<SDL_Renderer>& p_renderer,
    const std::shared_ptr<InputManager>& p_input_man)
  : p_doc_man_(p_doc_man), p_renderer_(p_renderer), p_input_man_(p_input_man) {
  p_main_menu_ = std::make_unique<UiMainMenu>(p_doc_man_);
  p_doc_tab_ = std::make_shared<UiDocumentTab>(p_renderer_, p_doc_man_);
  p_modal_dialogs_ = std::make_unique<UiModalDialogs>(p_doc_man_);
}

void UiManager::RenderMenu() {
  p_main_menu_->Render();
  show_unsaved_dialog_ = p_main_menu_->IsShowUnsavedDialog();
  stop_rendering_ = p_main_menu_->IsStopRendering();
  show_save_dialog_ = p_main_menu_->IsShowSaveDialog();
  show_load_dialog_ = p_main_menu_->IsShowLoadDialog();
  SetSharedVars();
}

void UiManager::RenderContent() {
  RenderTabs();
  if (const auto current_document = p_doc_man_->GetCurrentDocument()) {
    auto current_document_index = p_doc_man_->GetCurrentDocumentIndex();
    RenderTabContent(*current_document,  current_document_index);
    if (p_input_man_->HandleShortcuts() == ASCII_S && sort_request_ == false) {
      sort_request_ = true;
      p_doc_tab_->StartSort(*current_document,  current_document_index, [this](Document& document_document, uint64_t index_index) {
        p_doc_man_->SetDocOnIndex(document_document, index_index);
      });
    }
    if (p_input_man_->HandleShortcuts() == ASCII_A && new_tab_request_ == false && !p_doc_tab_->IsSorting()) {
      p_doc_tab_->AddNewEvent(*current_document);
      new_tab_request_ = true;
    }
    if (p_input_man_->HandleShortcuts() == 0 && new_tab_request_ == true) {
      new_tab_request_ = false;
    }
    if (p_input_man_->HandleShortcuts() == 0 && sort_request_ == true && p_doc_tab_->IsSorting() == false) {
      sort_request_ = false;
    }

    if (show_unsaved_dialog_) {
      p_modal_dialogs_->RenderUnsavedChanges();
      show_unsaved_dialog_ = p_modal_dialogs_->GetShowUnsavedDialog();
      show_save_dialog_ = p_modal_dialogs_->GetShowSaveDialog();
    }
    if (show_save_dialog_) {
      p_modal_dialogs_->RenderSaveDialog();
      show_save_dialog_ = p_modal_dialogs_->GetShowSaveDialog();
      p_main_menu_->SetShowSaveDialog(show_save_dialog_);
    }


  }
  if (show_load_dialog_) {
    p_modal_dialogs_->RenderLoadDialog();
    show_load_dialog_ = p_modal_dialogs_->GetShowLoadDialog();
    p_main_menu_->SetShowLoadDialog(show_load_dialog_);
  }
}

void UiManager::RenderTabs() {
  if (ImGui::BeginTabBar("DocumentTabs", ImGuiTabBarFlags_AutoSelectNewTabs)) {
    for (int32_t i = 0; i < p_doc_man_->DocumentSize(); ++i) {
      bool open = true;
      if (ImGui::BeginTabItem(p_doc_man_->GetSpecificDocument(i).name.c_str(),
            &open, ImGuiTabItemFlags_None)) {
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
          SetSharedVars();
        }
      }
    }
    if (ImGui::TabItemButton(
          "+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
      p_doc_man_->CreateNewDocument();
    }
    ImGui::EndTabBar();
  }
}

void UiManager::RenderTabContent(Document& doc, uint64_t index) const {
  p_doc_tab_->Render(doc, index);
}

void UiManager::SetShowUnsavedDialog(const bool show_unsaved_dialog) {
  show_unsaved_dialog_ = show_unsaved_dialog;
  SetSharedVars();
}

[[nodiscard]] bool UiManager::GetStopRendering() const {
  return stop_rendering_;
}

void UiManager::SetStopRendering(const bool stop_rendering) {
  stop_rendering_ = stop_rendering;
}

void UiManager::SetSharedVars() const {
  p_main_menu_->SetShowUnsavedDialog(show_unsaved_dialog_);
  p_modal_dialogs_->SetShowUnsavedDialog(show_unsaved_dialog_);
  p_modal_dialogs_->SetShowSaveDialog(show_save_dialog_);
  p_modal_dialogs_->SetShowLoadDialog(show_load_dialog_);
  if (show_unsaved_dialog_) {
    p_modal_dialogs_->SetShowUnsavedDialog(false);
    p_modal_dialogs_->RefreshDirectoryContents();
  }
}

std::shared_ptr<UiDocumentTab> UiManager::GetUiDocumentTab() const {
  return p_doc_tab_;
}

}  // namespace linea_one::ui