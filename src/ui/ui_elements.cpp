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
 * File: ui_elements.cpp
 * Created by kureii on 8/16/24
 */
#include "ui/ui_elements.h"

#include <iostream>
#include <ostream>

namespace linea_one::ui {

void UiElements::VerticalSeparator(const float height,
                                   const float x_offset,
                                   const float y_offset,
                                   const ImColor color) {
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec2 window_pos = ImGui::GetCursorPos();
  //ImVec2 window_size = ImGui::GetWindowSize();
  // Calculate start and end points
  auto p_start = ImVec2(window_pos.x + x_offset, window_pos.y + y_offset);
  auto p_end = ImVec2(p_start.x, p_start.y + height);

  // Ensure the separator stays within the window
  /*p_start.x = ImClamp(p_start.x, window_pos.x, window_pos.x + window_size.x);
  p_end.x = ImClamp(p_end.x, window_pos.x, window_pos.x + window_size.x);
  p_start.y = ImClamp(p_start.y, window_pos.y, window_pos.y + window_size.y);
  p_end.y = ImClamp(p_end.y, window_pos.y, window_pos.y + window_size.y);*/

  // Draw the line
  draw_list->AddLine(p_start, p_end, color);
}


}