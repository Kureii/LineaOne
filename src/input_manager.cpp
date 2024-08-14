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
 * File: input_manager.cpp
 * Created by kureii on 8/11/24
 */

#include <input_manager.h>

#include <SDL3/SDL_events.h>
#include <imgui_impl_sdl3.h>

#include <imgui.h>

namespace linea_one {
InputManager::InputManager(std::shared_ptr<DocumentManager> doc_man)
    : action_key(0), doc_man_(doc_man) {}

uint32_t InputManager::HandleShortcuts() {
  ImGuiIO const& io = ImGui::GetIO();
  if (io.KeyCtrl) {
    if (ImGui::IsKeyPressed(ImGuiKey_N) && action_key != ASCII_N) {
      doc_man_->CreateNewDocument();
      action_key = ASCII_N;  // ASCI N
    }
    if (ImGui::IsKeyPressed(ImGuiKey_W) && action_key != ASCII_W) {
      if (doc_man_->CloseDocumentWithCheck(
              doc_man_->GetCurrentDocumentIndex()) >= 0) {
        action_key = ASCII_W;  // ASCI W
      }
    }
  }
  if (ImGui::IsKeyReleased(ImGuiKey_N) || ImGui::IsKeyReleased(ImGuiKey_LeftCtrl) || ImGui::IsKeyReleased(ImGuiKey_RightCtrl)) {
    action_key = 0;
  }
  if (ImGui::IsKeyReleased(ImGuiKey_W) || ImGui::IsKeyReleased(ImGuiKey_LeftCtrl) || ImGui::IsKeyReleased(ImGuiKey_RightCtrl)) {
    action_key= 0;
  }
  return action_key;
}

bool InputManager::HandleEvents(SDL_Window* p_window_) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags.
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
      return true;
    }
    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
        event.window.windowID == SDL_GetWindowID(p_window_)) {
      return true;
    }
  }
  return false;
}

}