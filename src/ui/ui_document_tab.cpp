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

#include <imgui_internal.h>
#include <ui/ui_document_tab.h>
#include <ui/ui_elements.h>

#include <algorithm>

namespace linea_one::ui {

auto event = TimelineEvent{0, 1999, "I", false, "lorem ipsum"};

UiDocumentTab::UiDocumentTab(std::shared_ptr<SDL_Renderer> p_renderer)
    : p_renderer_(p_renderer) {
  p_drag_icon_ = std::make_unique<svg::SvgIcon>(
      RESOURCES_PATH "/icons/drag_indicator.svg", p_renderer.get());
}

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
  ImGui::BeginChild("LeftPanelTab", ImVec2(content_size.x, topPanelHeight),
                    false);
  // Add content for top panel here
  RenderEventBox(event);

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

void UiDocumentTab::RenderEventBox(TimelineEvent& event) {
  ImVec2 content_size = ImGui::GetContentRegionAvail();
  float container_height =
      event.expanded ? EVENT_CONTAINER_HEIGHT_EXPANDED : EVENT_CONTAINER_HEIGHT;

  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.02f, 0.02f, 0.02f, 1.0f));

  ImGui::BeginChild(("EventContainer_" + std::to_string(event.id)).c_str(),
                    ImVec2(content_size.x, container_height), true,
                    ImGuiWindowFlags_NoScrollbar);

  // Horizontal layout
  {
    // Drag indicator icon
    float icon_size = 24.0f;
    float icon_padding = 8.0f;
    ImVec2 icon_pos(icon_padding, container_height / 2 - icon_size / 2);
    p_drag_icon_->Draw(icon_pos, ImVec2(icon_size, icon_size));

    ImGui::SameLine(icon_size + icon_padding * 2);
    UiElements::VerticalSeparator(container_height+8, 8, event.expanded ? -56 : 0);

    ImGui::SameLine(icon_size + icon_padding * 2,8);

    // Content area
    {
      ImVec2 content_size_right_to_separator = ImGui::GetContentRegionAvail();
      float red_box_width = content_size_right_to_separator.x - 24.0f;
      float red_box_height = container_height - 8.0f;

      // Calculate the position to align the center of the red box with the top
      // of the container
      float red_box_y_offset = 8;

      ImGui::SetCursorPos(
          ImVec2(ImGui::GetCursorPosX() + 12, red_box_y_offset));


      ImGui::BeginChild(
          ("EventContainerTexts_" + std::to_string(event.id)).c_str(),
          ImVec2(red_box_width, red_box_height), false,
          ImGuiWindowFlags_NoScrollbar);

      {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::BeginChild(
            ("EventContainerTexts2_" + std::to_string(event.id)).c_str(),
            ImVec2(red_box_width, 48),
            false, ImGuiWindowFlags_NoScrollbar);
        {
          ImGui::Text("Date");
          char buf[64];
          ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
          ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
          ImGui::SetNextItemWidth(red_box_width - 16);
          ImGui::InputText("##DateInput", buf, 64);
          ImGui::PopStyleVar(2);
        }
        ImGui::EndChild();
        ImGui::PopStyleVar(2);
      }

      {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        ImGui::BeginChild(
            ("EventContainerTexts3_" + std::to_string(event.id)).c_str(),
            ImVec2(red_box_width, 48),
            false, ImGuiWindowFlags_NoScrollbar);
        {
          ImGui::Text("Headline");
          char buf[64];
          ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
          ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
          ImGui::SetNextItemWidth(red_box_width - 16);
          ImGui::InputText("##HeadlineInput", buf, 64);
          ImGui::PopStyleVar(2);
        }
        ImGui::EndChild();
        ImGui::PopStyleVar(2);

      }

      if (event.expanded) {
        ImGui::BeginChild(
            ("EventContainerTexts5_" + std::to_string(event.id)).c_str(),
            ImVec2(red_box_width, 96),
            false, ImGuiWindowFlags_NoScrollbar);
        {
          ImGui::Text("Description");
          char buf[256];
          ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
          ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
          ImGui::SetNextItemWidth(red_box_width - 16);
          ImGui::InputTextMultiline("##Description", buf, 256);
          ImGui::PopStyleVar(2);
        }
        ImGui::EndChild();
      }

      {
        ImGui::SetNextItemWidth(red_box_width);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + red_box_width / 2 - icon_padding * 2 - icon_padding);
        if (ImGui::ArrowButton(("expand_" + std::to_string(event.id)).c_str(),
                               event.expanded ? ImGuiDir_Up : ImGuiDir_Down)) {
          event.expanded = !event.expanded;
        }
      }
      ImGui::EndChild();
    }
  }

  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}

}  // namespace linea_one::ui