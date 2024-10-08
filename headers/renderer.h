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
 * File: renderer.h
 * Created by kureii on 8/14/24
 */
#pragma once

#include <SDL3/SDL.h>
#include <document_manager.h>
#include <ui/ui_manager.h>

#include <memory>

namespace linea_one {

class Renderer {
 public:
  Renderer(const std::shared_ptr<SDL_Window>& p_window,
    const std::shared_ptr<DocumentManager>& p_doc_man,
    const std::shared_ptr<InputManager>& p_input_man);
  bool Init();
  void Render() const;
  void SetShowUnsavedDialog(const bool show_unsaved_dialog) const;
  [[nodiscard]] std::shared_ptr<SDL_Renderer> GetSdlRenderer();
  [[nodiscard]] bool GetStopRendering() const;
  [[nodiscard]] std::shared_ptr<ui::UiManager> GetUiManager() const;

 private:
  std::shared_ptr<SDL_Window> p_window_;
  std::shared_ptr<SDL_Renderer> p_renderer_;
  std::shared_ptr<DocumentManager> p_doc_man_;
  std::shared_ptr<ui::UiManager> p_ui_man_;
  std::shared_ptr<InputManager> p_input_man_;
  bool show_unsaved_dialog_ = false;
};

}  // namespace linea_one
