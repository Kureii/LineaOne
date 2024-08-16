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
  const std::shared_ptr<DocumentManager> &p_doc_man)
  : p_doc_man_(p_doc_man) {}

void UiModalDialogs::RenderUnsavedChanges() {
  ImGui::OpenPopup("Unsaved Changes");

  if (ImGui::BeginPopupModal(
        "Unsaved Changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Document '%s' has unsaved changes. Do you want to save before "
                "closing?",
      p_doc_man_->GetSpecificDocument(p_doc_man_->GetDocToClose())
        .name.c_str());
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
void UiModalDialogs::SetShowUnsavedDialog(const bool show_unsaved_dialog) {
  show_unsaved_dialog_ = show_unsaved_dialog;
}

bool UiModalDialogs::GetShowUnsavedDialog() const {
  return show_unsaved_dialog_;
}

}  // namespace linea_one::ui