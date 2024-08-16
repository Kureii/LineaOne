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

#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>

namespace linea_one::ui::elements {

void VerticalSeparator(const float height, const float x_offset,
  const float y_offset, const ImColor color) {
  ImVec2 p_start = ImGui::GetCursorScreenPos();
  p_start.x += x_offset;
  p_start.y += y_offset;

  // Konec čáry je na stejném x, ale vyšší o "height"
  ImVec2 p_end = ImVec2(p_start.x, p_start.y + height);

  // Získej ImDrawList a přidej čáru
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddLine(p_start, p_end, color,
    1.0f);  // Poslední argument je šířka čáry (1 pixel)

  // Posuň kurzor dolů, pokud potřebuješ další vykreslování pod separátorem
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y_offset + height);
}

void RenderIcon(std::shared_ptr<svg::SvgIcon> icon, float icon_padding,
  float icon_height, float icon_width, ImVec2 cursor_pos) {
  ImVec2 icon_pos(cursor_pos.x + icon_padding,
    cursor_pos.y + icon_height / 2 - icon_width / 2);
  if (icon_height > icon_width) {
    icon->Draw(icon_pos, ImVec2(icon_width, icon_width));
  } else {
    icon->Draw(icon_pos, ImVec2(icon_height, icon_height));
  }
}

bool RenderIconButton(std::string& name, std::shared_ptr<svg::SvgIcon> icon,
  float icon_padding, float icon_height, float icon_width, ImVec2 icon_pos,
  float button_padding,
   float button_height, float button_width, ImVec2 button_pos,
  const std::function<void()>& callback) {
  ImGui::SetCursorPos(
    ImVec2(button_pos.x + button_padding, button_pos.y + button_padding));
  bool clicked =
    ImGui::Button(name.c_str(), ImVec2(button_width, button_height));
  ImGui::SetCursorPos(
    ImVec2(icon_pos.x + icon_padding * 2, icon_pos.y + icon_padding * 2));
  RenderIcon(icon, icon_padding, icon_height, icon_width, icon_pos);

  if (clicked && callback) {
    callback();
  }

  return clicked;
}

}  // namespace linea_one::ui::elements