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

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <memory>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

namespace linea_ona {

class App {
public:
  App();
  ~App();

  bool Init();
  void Run();
private:
  void ProccessEvents();
  void Update();
  void Render();

  bool CreateWindow();
  bool CreateRenderer();

  void SetupImGui();

  void RenderMenu();
  void RenderContent();

  SDL_Window* p_window_;
  SDL_Renderer* p_renderer_;
  bool stop_;
  ImVec4 clear_color_;
};

} // linea_ona
