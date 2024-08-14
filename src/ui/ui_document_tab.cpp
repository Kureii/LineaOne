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
#include <imgui.h>
#include <ui/ui_document_tab.h>

#include <algorithm>

namespace linea_one::ui {

void UiDocumentTab::Render(Document& document) {
  ImVec2 contentSize = ImGui::GetContentRegionAvail();

  static float leftPanelWidth = 400.0f;

  float minWidth = 400.0f;
  float maxWidth = contentSize.x * 0.5f;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.3f, 0.0f, 1.0f));
  ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, contentSize.y), true);

  ImGui::Text("Left panel");
  // Left panel content

  ImGui::EndChild();
  ImGui::PopStyleColor();

  // Resize slider
  ImGui::SameLine(0, 0);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.1f));
  ImGui::Button("##Splitter", ImVec2(8.0f, contentSize.y));
  ImGui::PopStyleColor(3);

  if (ImGui::IsItemActive()) {
    float mousePositionX = ImGui::GetIO().MousePos.x;
    float leftPanelEdgeX = ImGui::GetItemRectMin().x;
    leftPanelWidth = mousePositionX - ImGui::GetWindowPos().x;
    leftPanelWidth = std::clamp(leftPanelWidth, minWidth, maxWidth);
  }

  // Cursor
  if (ImGui::IsItemHovered() || ImGui::IsItemActive())
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

  // Right panel
  ImGui::SameLine(0, 0);  // Žádná mezera mezi táhlem a pravým panelem
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.3f, 1.0f));
  ImGui::BeginChild("RightPanel", ImVec2(0, contentSize.y), true);

  ImGui::Text("Right panel");
  // Right panel content

  ImGui::EndChild();
  ImGui::PopStyleColor();
}

} // linea_one::ui