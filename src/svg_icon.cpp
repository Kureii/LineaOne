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
 * File: svg_icon.cpp
 * Created by kureii on 8/15/24
 */
#include <svg_icon.h>
#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#define NANOSVGRAST_IMPLEMENTATION
#include <nanosvgrast.h>

namespace linea_one::svg {

SvgIcon::SvgIcon(const std::string& filename, SDL_Renderer* p_renderer)
  : p_renderer_(p_renderer), p_texture_(nullptr) {
  NSVGimage* image = nsvgParseFromFile(filename.c_str(), "px", 96);
  if (image == nullptr) {
    SDL_Log("Error loading SVG: %s", filename.c_str());
    return;
  }

  width_ = static_cast<int>(image->width);
  height_ = static_cast<int>(image->height);

  NSVGrasterizer* rast = nsvgCreateRasterizer();
  unsigned char* pixels = new unsigned char[width_ * height_ * 4];
  nsvgRasterize(rast, image, 0, 0, 1, pixels, width_, height_, width_ * 4);

  // Vytvoření SDL_Surface s použitím aktuální verze SDL_CreateSurfaceFrom
  SDL_Surface* surface = SDL_CreateSurfaceFrom(
    width_, height_, SDL_PIXELFORMAT_RGBA32, pixels, width_ * 4);
  if (surface) {
    p_texture_ = SDL_CreateTextureFromSurface(p_renderer_, surface);
    SDL_DestroySurface(surface);
  } else {
    SDL_Log("Error creating surface: %s", SDL_GetError());
  }

  delete[] pixels;
  nsvgDeleteRasterizer(rast);
  nsvgDelete(image);
}

SvgIcon::~SvgIcon() {
  if (p_texture_) {
    SDL_DestroyTexture(p_texture_);
  }
}

void SvgIcon::Draw(ImVec2 pos, ImVec2 size) {
  if (p_texture_ != nullptr) {
    ImGui::SetCursorPos(pos);
    ImGui::Image(p_texture_, size);
  }
}

}  // namespace linea_one::svg