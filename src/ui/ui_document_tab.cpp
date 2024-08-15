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
 * File: ui_document_tab.cpp
 * Created by kureii on 8/14/24
 */

#include <ui/ui_document_tab.h>

#include <algorithm>

namespace linea_one::ui {

void UiDocumentTab::Render(Document& document) {
  ImVec2 content_size = ImGui::GetContentRegionAvail();

  static float leftPanelWidth = 400.0f;

  float minWidth = 400.0f;
  float maxWidth = content_size.x * 0.5f;
  leftPanelWidth = std::clamp(leftPanelWidth, minWidth, maxWidth);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);

  ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, content_size.y), true);

  RenderLeftBox(document);

  ImGui::EndChild();

  // Resize slider
  ImGui::SameLine(0, 0);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.1f));
  ImGui::Button("##Splitter", ImVec2(8.0f, content_size.y));
  ImGui::PopStyleColor(3);

  if (ImGui::IsItemActive()) {
    float mousePositionX = ImGui::GetIO().MousePos.x;
    leftPanelWidth = mousePositionX - ImGui::GetWindowPos().x;
    leftPanelWidth = std::clamp(leftPanelWidth, minWidth, maxWidth);
  }

  // Cursor
  if (ImGui::IsItemHovered() || ImGui::IsItemActive())
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

  // Right panel
  ImGui::SameLine(0, 0);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.3f, 1.0f));
  ImGui::BeginChild("RightPanel", ImVec2(0, content_size.y), true);

  RenderRightBox(document);

  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar(2);
}

void UiDocumentTab::RenderLeftBox(Document& document) {
  ImVec2 content_size = ImGui::GetContentRegionAvail();

  // Top panel
  float topPanelHeight = content_size.y - 34.0f;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
  ImGui::BeginChild("LeftPanelTab", ImVec2(content_size.x, topPanelHeight), false);
  // Add content for top panel here
  ImGui::EndChild();
  ImGui::PopStyleColor();

  // Bottom panel
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
  ImGui::BeginChild("LeftPanelSort", ImVec2(content_size.x, 30.0f), false);
  ImVec2 content_size_button = ImGui::GetContentRegionAvail();
  ImGui::Button("Sort", ImVec2(content_size_button.x, content_size_button.y));
  ImGui::EndChild();
  ImGui::PopStyleColor();
}

void UiDocumentTab::RenderRightBox(Document& document) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::Text("Right panel");
    ImGui::PopStyleColor();
}


} // linea_one::ui