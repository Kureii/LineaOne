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
 * File: ui_draw_timeline.cpp
 * Created by kureii on 8/19/24
 */
#include <imgui.h>
#include <ui/ui_draw_timeline.h>

#include <algorithm>
#include <format>

namespace linea_one::ui {

void UiDrawTimeline::Render(
  const std::vector<TimelineEvent>& events, TimelineState& state) {
  HandleInteraction(state, events.size());
  DrawTimeline(events, state);
}

void UiDrawTimeline::DrawTimeline(
  const std::vector<TimelineEvent>& events, TimelineState& state) {
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
  ImVec2 canvas_size = ImGui::GetContentRegionAvail();

  // Set clipping rectangle
  draw_list->PushClipRect(canvas_pos,
    ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);

  if (events.size() == 1) {
    // Special case for single event
    state.zoom = 1.0f;
    state.offset = 0.0f;

    const auto& event = events[0];
    ImVec2 center(
      canvas_pos.x + canvas_size.x / 2, canvas_pos.y + canvas_size.y / 2);

    // Draw point
    draw_list->AddCircleFilled(center, 5.0f, IM_COL32(0, 120, 250, 255));

    // Draw year
    std::string year_str = std::to_string(event.year);
    ImVec2 year_text_size = ImGui::CalcTextSize(year_str.c_str());
    ImVec2 year_pos(center.x - year_text_size.x / 2, center.y + 10);
    draw_list->AddText(
      year_pos, IM_COL32(200, 200, 200, 255), year_str.c_str());

    // Draw headline
    ImVec2 headline_text_size = ImGui::CalcTextSize(event.headline.c_str());
    ImVec2 headline_pos(center.x - headline_text_size.x / 2, center.y - 25);
    draw_list->AddText(
      headline_pos, IM_COL32(255, 255, 255, 255), event.headline.c_str());
  } else {
    ImVec2 start(canvas_pos.x, canvas_pos.y + canvas_size.y / 2);
    ImVec2 end(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y / 2);
    draw_list->AddLine(start, end, IM_COL32(255, 255, 255, 255), 2.0f);

    for (const auto& event : events) {
      float x = MapYearToPixel(event.year, state, canvas_pos.x, canvas_size.x);

      // Skip events outside the visible area
      if (x < canvas_pos.x - 50 || x > canvas_pos.x + canvas_size.x + 50) {
        continue;
      }

      ImVec2 point_pos(x, canvas_pos.y + canvas_size.y / 2);

      // Draw point
      draw_list->AddCircleFilled(point_pos, 5.0f, IM_COL32(0, 120, 250, 255));

      // Draw year
      ImVec2 year_text_pos(x - 10, point_pos.y + 10);
      if (year_text_pos.x >= canvas_pos.x &&
          year_text_pos.x + 20 <= canvas_pos.x + canvas_size.x) {
        draw_list->AddText(year_text_pos, IM_COL32(200, 200, 200, 255),
          std::to_string(event.year).c_str());
      }

      // Draw headline
      ImVec2 text_size = ImGui::CalcTextSize(event.headline.c_str());
      ImVec2 headline_text_pos(x - text_size.x / 2, point_pos.y - 25);
      if (headline_text_pos.x >= canvas_pos.x &&
          headline_text_pos.x + text_size.x <= canvas_pos.x + canvas_size.x) {
        draw_list->AddText(headline_text_pos, IM_COL32(255, 255, 255, 255),
          event.headline.c_str());
      }
    }
  }
  // Draw main axis

  // Pop clipping rectangle
  draw_list->PopClipRect();
}

float UiDrawTimeline::MapYearToPixel(
  int year, const TimelineState& state, float startX, float width) {
  float yearRange = state.maxYear - state.minYear;
  float normalizedYear = (year - state.minYear) / yearRange;
  return startX + (normalizedYear * width * (state.zoom*0.8)) + state.offset + 85;
}

void UiDrawTimeline::HandleInteraction(TimelineState& state, uint64_t events_size) {
  if (events_size <= 1) {
    return; // No interaction for single or no events
  }

  if (ImGui::IsWindowHovered()) {
    // Zoom
    float wheel = ImGui::GetIO().MouseWheel;
    if (wheel != 0) {
      state.zoom *= (1 + wheel * 0.1f);
      state.zoom = std::max(0.1f, std::min(state.zoom, 10.0f));
    }

    // Pan
    if (ImGui::IsMouseDragging(0)) {
      state.offset += ImGui::GetIO().MouseDelta.x;
    }
  }
}

}  // namespace linea_one::ui