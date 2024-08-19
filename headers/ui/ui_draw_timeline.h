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
 * File: ui_draw_timeline.h
 * Created by kureii on 8/19/24
 */
#pragma once

#include <document.h>
#include <timeline_state.h>

namespace linea_one::ui {

struct TextPosition {
  ImVec2 pos;
  float width;
};

class UiDrawTimeline {
 public:
  UiDrawTimeline() = default;
  static void Render(const std::vector<TimelineEvent>& events, TimelineState& state);

private:
  static void DrawTimeline(const std::vector<TimelineEvent>& events, TimelineState& state);
  static float MapYearToPixel(int year, const TimelineState& state, float startX, float width);
  static void HandleInteraction(TimelineState& state, uint64_t events_size);
};

}  // namespace linea_one::ui
