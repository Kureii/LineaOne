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
 * File: ExportDocument.h
 * Created by kureii on 8/31/24
 */
#pragma once

#include <timeline_event.h>
#include <timeline_state.h>
#include <vector>
#include <filesystem>

namespace linea_one {

class ExportDocument {
  public:
  ExportDocument() = default;

  std::string ExportTimelineToSVG(const std::vector<TimelineEvent>& events, const TimelineState& state);
  void SaveTimelineAsSVG(const std::vector<TimelineEvent>& events, const TimelineState& state, const std::filesystem::path path);

};

}
