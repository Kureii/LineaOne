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
 * File: ui_elements.h
 * Created by kureii on 8/16/24
 */
#pragma once
#include <imgui.h>
#include <imgui_internal.h>

namespace linea_one::ui {
class UiElements {
 public:
  explicit UiElements() = default;
  virtual ~UiElements() = default;
  static void VerticalSeparator(
      float height, float x_offset = 0.0f, float y_offset = 0.0f,
      ImColor color = ImGui::GetStyle().Colors[ImGuiCol_Separator]);
};
}  // namespace linea_one::ui