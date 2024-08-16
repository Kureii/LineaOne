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
 * File: svg_icon.h
 * Created by kureii on 8/15/24
 */
#pragma once

#include <string>
#include <memory>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

namespace linea_one::svg {


class SvgIcon {
 public:
  SvgIcon(const std::string& filename, SDL_Renderer* p_renderer);
 ~SvgIcon();
 void Draw(ImVec2 pos, ImVec2 size);
 private:
 SDL_Renderer* p_renderer_;
 SDL_Texture* p_texture_;
 int width_, height_;
};

} // linea_one::svg
