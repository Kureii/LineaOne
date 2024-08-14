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
 * File: renderer.cpp
 * Created by kureii on 8/14/24
 */
#include "renderer.h"
#include <imgui.h>
#include <imgui_impl_sdlrenderer3.h>

namespace linea_one {

Renderer::Renderer(const std::shared_ptr<SDL_Window>& p_window,
                   const std::shared_ptr<DocumentManager>& doc_man)
    : p_window_(p_window), doc_man_(doc_man) {}

Renderer::~Renderer() {
  if (p_renderer_ != nullptr) {
    ImGui_ImplSDLRenderer3_Shutdown();

  }
}

bool Renderer::Init() {
  p_renderer_ = std::shared_ptr<SDL_Renderer>(
        SDL_CreateRenderer(p_window_.get(), nullptr),
        SDL_DestroyRenderer);
  SDL_SetRenderVSync(p_renderer_.get(), 1);

  if (p_renderer_ == nullptr) {
    SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
    return false;
  }
  std::cout << p_renderer_.get() << std::endl;
  return true;
}

bool Renderer::Render() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("Fullscreen Window", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_MenuBar);
  bool stop = RenderMenu();
  RenderContent();

  ImGui::End();

  ImGui::Render();
  SDL_SetRenderDrawColor(p_renderer_.get(), 0, 0, 0, 255);
  SDL_RenderClear(p_renderer_.get());
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),
                                        p_renderer_.get());
  SDL_RenderPresent(p_renderer_.get());

  return stop;
}

bool Renderer::RenderMenu() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New file", "Ctrl+N")) {
        doc_man_->CreateNewDocument();
      }
      if (ImGui::MenuItem("Open file")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Save")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Save as")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Close file", "Ctrl+W")) {
        if (doc_man_->CloseDocumentWithCheck(doc_man_->GetCurrentDocumentIndex()) >= 0) {
          show_unsaved_dialog_ = true;
        }
      }
      if (ImGui::MenuItem("Exit")) {
        return true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
  return false;
}

void Renderer::RenderContent() {
  RenderTabs();
  if (auto current_document = doc_man_->GetCurrentDocument()) {
    RenderTabContent(*current_document);
  }

  if (show_unsaved_dialog_) {
    RenderUnsavedChangesDialog();
  }
}

void Renderer::RenderTabs() {
  if (ImGui::BeginTabBar("DocumentTabs", ImGuiTabBarFlags_AutoSelectNewTabs)) {
    for (uint32_t i = 0; i < doc_man_->DocumentSize(); ++i) {
      bool open = true;
      if (ImGui::BeginTabItem(doc_man_->GetSpecificDocument(i).name.c_str(), &open, ImGuiTabItemFlags_None)) {
        doc_man_->SetCurrentDocumentIndex(i);
        ImGui::EndTabItem();
      }
      if (!open) {
        auto index_to_close = doc_man_->CloseDocumentWithCheck(i);
        if (index_to_close == -1) {
          i--;
        }
        if (index_to_close > -1) {
          show_unsaved_dialog_ = true;
        }
      }
    }
    if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
      doc_man_->CreateNewDocument();
    }
    ImGui::EndTabBar();
  }
}

void Renderer::RenderTabContent(const Document& doc) {
  ImGui::Text("Content of document: %s", doc.name.c_str());
}

void Renderer::RenderUnsavedChangesDialog() {
  ImGui::OpenPopup("Unsaved Changes");

  if (ImGui::BeginPopupModal("Unsaved Changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Document '%s' has unsaved changes. Do you want to save before closing?",
                doc_man_->GetSpecificDocument(doc_man_->GetDocToClose()).name.c_str());
    ImGui::Separator();

    if (ImGui::Button("Save", ImVec2(120, 0))) {
      /* TODO save function */
      doc_man_->GetSpecificDocument(doc_man_->GetDocToClose()).saved = true;
      doc_man_->CloseDocument();
      ImGui::CloseCurrentPopup();
      show_unsaved_dialog_ = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Don't Save", ImVec2(120, 0))) {
      doc_man_->CloseDocument();
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

void Renderer::SetShowUnsavedDialog(const bool show_unsaved_dialog) {
  show_unsaved_dialog_ = show_unsaved_dialog;
}

std::shared_ptr<SDL_Renderer> Renderer::GetSdlRenderer() {
  return p_renderer_;
}

}  // namespace linea_one