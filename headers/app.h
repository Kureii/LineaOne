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
 * File: app.h
 * Created by kureii on 8/11/24
 */
#pragma once

#include <SDL3/SDL.h>
#include <input_manager.h>

#include <memory>
#include <renderer.h>

#include <document_manager.h>
#include <imgui.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

namespace linea_one {

class App {
public:
  App();
  ~App();

  bool Init();
  void Run();
private:
  static void Update();

  bool CreateWindow();

  void SetupImGui() const;

  bool stop_;
  std::shared_ptr<SDL_Window> p_window_;
  ImVec4 clear_color_;
  std::shared_ptr<DocumentManager> p_doc_man_;
  std::shared_ptr<Renderer> p_renderer_;
  std::unique_ptr<InputManager> p_input_man_;

  bool new_doc_finised_ = false;
  bool close_doc_finised_ = false;
  int new_doc_counter = 1;
};

} // linea_ona
