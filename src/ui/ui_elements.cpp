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

#include <imgui_internal.h>

#include <functional>
#include <iostream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>

namespace linea_one::ui::elements {

void VerticalSeparator(const float height, const float x_offset,
  const float y_offset, const ImColor color) {
  ImVec2 p_start = ImGui::GetCursorScreenPos();
  p_start.x += x_offset;
  p_start.y += y_offset;

  ImVec2 p_end = ImVec2(p_start.x, p_start.y + height);

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddLine(p_start, p_end, color,
    1);

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
  if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

bool RenderIconButton(std::string& name, std::shared_ptr<svg::SvgIcon> icon,
  float icon_padding, float icon_height, float icon_width, ImVec2 icon_pos,
  float button_padding, float button_height, float button_width,
  ImVec2 button_pos, const std::function<void()>& callback) {
  ImGui::SetCursorPos(
    ImVec2(button_pos.x + button_padding, button_pos.y + button_padding));
  bool clicked =
    ImGui::Button(name.c_str(), ImVec2(button_width, button_height));
  if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  ImGui::SetCursorPos(
    ImVec2(icon_pos.x + icon_padding * 2, icon_pos.y + icon_padding * 2));
  RenderIcon(icon, icon_padding, icon_height, icon_width, icon_pos);

  if (clicked && callback) {
    callback();
  }

  return clicked;
}

void RenderSpinner(
  const char* label, const char* display_text, float radius, int thickness, float speed, float arc_length, ImVec4 color) {
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);

  ImVec2 pos = window->DC.CursorPos;
  ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

  const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
  ImGui::ItemSize(bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(bb, id))
    return;

  window->DrawList->PathClear();

  int num_segments = 30;
  float start = (float)ImGui::GetTime() * speed;
  start = fmodf(start, 2.0f * M_PI);

  const float a_min = start;
  const float a_max = a_min + arc_length * 2.0f * M_PI;

  const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

  for (int i = 0; i <= num_segments; i++)
  {
    const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
    window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius,
                                        centre.y + ImSin(a) * radius));
  }

  window->DrawList->PathStroke(ImGui::GetColorU32(color), false, (float)thickness);

  auto text_size = ImGui::CalcTextSize(display_text);
  if (text_size.x > radius) {
    ImGui::SetCursorPos(ImVec2(pos.x - (text_size.x - radius)/2 , pos.y + thickness+2));
  } else {
    ImGui::SetCursorPos(ImVec2(pos.x - radius / 2 , pos.y + thickness+2));
  }
  ImGui::Text(display_text);
}

}  // namespace linea_one::ui::elements