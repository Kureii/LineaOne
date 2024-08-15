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
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <renderer.h>

namespace linea_one {

Renderer::Renderer(const std::shared_ptr<SDL_Window>& p_window,
                   const std::shared_ptr<DocumentManager>& doc_man)
    : p_window_(p_window), p_doc_man_(doc_man) {
  p_ui_man_ = std::make_shared<ui::UiManager>(p_doc_man_);
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
  return true;
}

void Renderer::Render() const {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("Fullscreen Window", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_MenuBar);
  p_ui_man_->RenderMenu();
  p_ui_man_->RenderContent();
  p_ui_man_->SetSharedVars();

  ImGui::End();

  ImGui::Render();
  SDL_SetRenderDrawColor(p_renderer_.get(), 0, 0, 0, 255);
  SDL_RenderClear(p_renderer_.get());
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),
                                        p_renderer_.get());
  SDL_RenderPresent(p_renderer_.get());

}

void Renderer::SetShowUnsavedDialog(const bool show_unsaved_dialog) const {
  p_ui_man_->SetShowUnsavedDialog(show_unsaved_dialog);
}

std::shared_ptr<SDL_Renderer> Renderer::GetSdlRenderer() {
  return p_renderer_;
}

[[nodiscard]] bool Renderer::GetStopRendering() const {
  return p_ui_man_->GetStopRendering();
}

}  // namespace linea_one::ui