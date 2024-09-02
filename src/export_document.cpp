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
#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace linea_one {

struct EventPosition {
  float x;
  float y;
  int year;
};

std::string utf8ToXMLEntities(const std::string& input) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  std::u32string utf32 = converter.from_bytes(input);

  std::ostringstream result;
  for (char32_t c : utf32) {
    if (c <= 127) {
      switch (c) {
        case '&':  result << "&amp;";  break;
        case '\"': result << "&quot;"; break;
        case '\'': result << "&apos;"; break;
        case '<':  result << "&lt;";   break;
        case '>':  result << "&gt;";   break;
        default:   result << static_cast<char>(c); break;
      }
    } else {
      result << "&#" << static_cast<uint32_t>(c) << ";";
    }
  }
  return result.str();
}

std::string ExportDocument::ExportTimelineToSVG(
  const std::vector<TimelineEvent>& events, const TimelineState& state) {
  const float minPointWidth = 100.0f;
  const float padding = 20.0f;
  const int baseHeight = 250;
  const float textHeight = 20.0f;
  const float yearTextY = baseHeight / 2 + 20;
  const float circleY = baseHeight / 2;

  auto getTextWidth = [](const std::string& text) {
    return ImGui::CalcTextSize(text.c_str()).x;
  };

  std::map<int, std::vector<const TimelineEvent*>> eventsByYear;
  for (const auto& event : events) {
    eventsByYear[event.year].push_back(&event);
  }

  float totalWidth = 0.0f;
  std::map<int, float> yearXPositions;
  for (const auto& [year, yearEvents] : eventsByYear) {
    float maxWidth = minPointWidth;
    for (const auto* event : yearEvents) {
      float headlineWidth = getTextWidth(event->headline);
      maxWidth = std::max(maxWidth, headlineWidth);
    }
    float width = maxWidth + padding;
    yearXPositions[year] = totalWidth + width / 2;
    totalWidth += width;
  }

  int maxEventsInYear = 0;
  for (const auto& [year, yearEvents] : eventsByYear) {
    maxEventsInYear = std::max(maxEventsInYear, static_cast<int>(yearEvents.size()));
  }
  int svgHeight = baseHeight + (maxEventsInYear - 1) * textHeight;

  std::ostringstream svg;
  svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  svg << R"(<svg xmlns="http://www.w3.org/2000/svg" width=")" << totalWidth << R"(" height=")" << svgHeight
      << R"(" viewBox="0 0 )" << totalWidth << " " << svgHeight << R"(">
<style>
  text { font-family: Arial, sans-serif; }
  .year { fill: #1d1d1d; font-size: 12px; }
  .headline { fill: #1d1d1d; font-size: 14px; }
  .description { display: none; }
</style>
)";

  svg << R"(<line x1="0" y1=")" << circleY << R"(" x2=")" << totalWidth
      << R"(" y2=")" << circleY << R"(" stroke="#1d1d1d" stroke-width="2" />
)";

  for (const auto& [year, yearEvents] : eventsByYear) {
    float x = yearXPositions[year];
    for (size_t i = 0; i < yearEvents.size(); ++i) {
      const auto* event = yearEvents[i];
      float textY = circleY - 15 - (i * textHeight);

      svg << "<g>\n"
          << "  <circle cx=\"" << x << "\" cy=\"" << circleY << "\" r=\"5\" fill=\"#0078fa\" />\n"
          << "  <text x=\"" << x << "\" y=\"" << yearTextY
          << "\" text-anchor=\"middle\" class=\"year\">" << year << "</text>\n"
          << "  <text x=\"" << x << "\" y=\"" << textY
          << "\" text-anchor=\"middle\" class=\"headline\">" << utf8ToXMLEntities(event->headline) << "</text>\n"
          << "  <text x=\"" << x << "\" y=\"0\" class=\"description\">" << utf8ToXMLEntities(event->description) << "</text>\n"
          << "</g>\n";
    }
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
