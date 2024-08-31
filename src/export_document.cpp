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
 * File: ExportDocument.cpp
 * Created by kureii on 8/31/24
 */
#include <export_document.h>
#include <fstream>

namespace linea_one {

std::string ExportDocument::ExportTimelineToSVG(
  const std::vector<TimelineEvent>& events, const TimelineState& state) {
  std::ostringstream svg;
  svg << R"(<svg xmlns="http://www.w3.org/2000/svg" width="800" height="200" viewBox="0 0 800 200">\n)";
  svg << "<style>\n"
      << "  text { font-family: Arial, sans-serif; }\n"
      << "  .year { fill: #1d1d1d; font-size: 12px; }\n"
      << "  .headline { fill: #1d1d1d; font-size: 14px; }\n"
      << "</style>\n";

  // Main timeline axis
  svg << R"(<line x1="50" y1="100" x2="750" y2="100" stroke="#1d1d1d" stroke-width="2" />\n)";

  int minYear = events.empty() ? 0 : events[0].year;
  int maxYear = events.empty() ? 0 : events[0].year;
  for (const auto& event : events) {
    minYear = std::min(minYear, event.year);
    maxYear = std::max(maxYear, event.year);
  }

  auto mapYearToX = [&](int year) {
    float normalizedYear = (year - minYear) / static_cast<float>(maxYear - minYear);
    return 50 + (normalizedYear * 700 * state.zoom) + state.offset;
  };

  for (const auto& event : events) {
    float x = mapYearToX(event.year);

    svg << "<circle cx=\"" << x << "\" cy=\"100\" r=\"5\" fill=\"#0078fa\" />\n";
    svg << "<text x=\"" << x << R"(" y="120" text-anchor="middle" class="year">)" << event.year << "</text>\n";
    svg << "<text x=\"" << x << R"(" y="85" text-anchor="middle" class="headline">)" << event.headline << "</text>\n";
  }

  svg << "</svg>";
  return svg.str();
}
void ExportDocument::SaveTimelineAsSVG(const std::vector<TimelineEvent>& events,
  const TimelineState& state, const std::filesystem::path path) {
  std::string svgContent = ExportTimelineToSVG(events, state);
  std::ofstream file(path);
  if (file.is_open()) {
    file << svgContent;
    file.close();
    std::cout << "SVG timeline saved to " << path << std::endl;
  } else {
    std::cerr << "Unable to open file for writing: " << path << std::endl;
  }
}

}  // namespace linea_one
