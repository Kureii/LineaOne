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
#include <format>

namespace linea_one::ui {

UiDocumentTab::UiDocumentTab(const std::shared_ptr<SDL_Renderer>& p_renderer)
  : p_renderer_(p_renderer), year_(1999), new_year_(1999) {
  p_drag_icon_ =
    std::make_shared<svg::SvgIcon>(DRAG_INDICATOR_ICON_PATH, p_renderer.get());
  p_delete_icon_ =
    std::make_shared<svg::SvgIcon>(DELETE_FOREVER_ICON_PATH, p_renderer.get());
  p_arrow_drop_up_icon_ =
    std::make_shared<svg::SvgIcon>(ARROW_DROP_UP_ICON_PATH, p_renderer.get());
  p_arrow_drop_down_icon_ =
    std::make_shared<svg::SvgIcon>(ARROW_DROP_DOWN_ICON_PATH, p_renderer.get());
  a_buffer_headline_ = new char[BUFFER_HEADLINE_SIZE];
  for (auto i = 0; i < BUFFER_HEADLINE_SIZE; i++) {
    a_buffer_headline_[i] = '\0';
  }
  a_buffer_description_ = new char[BUFFER_DESCRIPTION_SIZE];
  for (auto i = 0; i < BUFFER_DESCRIPTION_SIZE; i++) {
    a_buffer_description_[i] = '\0';
  }
}

UiDocumentTab::~UiDocumentTab() {
  if (a_buffer_headline_) delete[] a_buffer_headline_;
  if (a_buffer_description_) delete[] a_buffer_description_;
}

void UiDocumentTab::Render(Document& document) {
  const ImVec2 content_size = ImGui::GetContentRegionAvail();

  constexpr float minWidth = MIN_SIZE_LEFT_PANEL;
  const float maxWidth = content_size.x * 0.5f;
  left_panel_width_ = std::clamp(left_panel_width_, minWidth, maxWidth);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);

  ImGui::BeginChild(
    "LeftPanel", ImVec2(left_panel_width_, content_size.y), true);

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
    left_panel_width_ = mousePositionX - ImGui::GetWindowPos().x;
    left_panel_width_ = std::clamp(left_panel_width_, minWidth, maxWidth);
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

void UiDocumentTab::AddNewEvent(Document& document) {
  document.events.emplace_back(last_id_, new_year_, "", false, "");
  last_id_++;
  new_year_++;
}

void UiDocumentTab::RenderLeftBox(Document& document) {
  ImVec2 content_size = ImGui::GetContentRegionAvail();

  float topPanelHeight = content_size.y - 34.0f;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
  ImGui::BeginChild(
    "LeftPanelTab", ImVec2(content_size.x, topPanelHeight), false);
  if (document.events.empty()) {
    document.events.emplace_back(last_id_, new_year_, "", false, "");
    last_id_++;
    new_year_++;
  }
  for (uint64_t i = 0; i < document.events.size(); ++i) {
    RenderEventBox(document, document.events[i], i);
  }

  if (ImGui::Button("Add", ImVec2(content_size.x, 20))) {
    AddNewEvent(document);
  }
  if (ImGui::IsItemHovered(
        ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
    ImGui::BeginTooltip();
    ImGui::TextUnformatted("Shift+A");
    ImGui::EndTooltip();
  }

  ImGui::EndChild();
  ImGui::PopStyleColor();

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

void UiDocumentTab::RenderEventBox(
  Document& document, TimelineEvent& event, uint64_t order) {
  const ImVec2 content_size = ImGui::GetContentRegionAvail();
  const float container_height =
    event.expanded ? EVENT_CONTAINER_HEIGHT_EXPANDED : EVENT_CONTAINER_HEIGHT;

  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

  const ImVec4 bg_color(0.15f, 0.15f, 0.15f, 1.0f);  // Tmavě šedá barva pozadí
  const ImVec4 border_color(
    0.3f, 0.3f, 0.3f, 1.0f);  // Světlejší šedá pro obrys

  ImGui::PushStyleColor(ImGuiCol_ChildBg, bg_color);
  ImGui::PushStyleColor(ImGuiCol_Border, border_color);

  ImGui::BeginChild(
    std::format("EventContainer_{}", std::to_string(event.id)).c_str(),
    ImVec2(content_size.x, container_height), true,
    ImGuiWindowFlags_NoScrollbar);

  elements::RenderIcon(
    p_drag_icon_, ICON_PADDING, container_height, ICON_SIZE, ImVec2(-2, 0));

  ImGui::SetCursorPos(ImVec2(-2, 0));
  ImGui::InvisibleButton(
    "##DragHandle", ImVec2(ICON_SIZE + ICON_PADDING * 2, container_height));

  if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
    ImGui::SetDragDropPayload("EVENT_DND", &order, sizeof(int));

    std::string formatted_text = std::format("Moving Event {}{}", event.year,
      event.headline.empty() ? "" : std::format(", {}", event.headline));
    ImGui::Text("%s", formatted_text.c_str());

    ImGui::EndDragDropSource();
  }

  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
          ImGui::AcceptDragDropPayload("EVENT_DND")) {
      int source_order = *(const int*)payload->Data;
      SwapEvents(document, source_order, order);
    }
    ImGui::EndDragDropTarget();
  }

  ImGui::SameLine(ICON_SIZE + ICON_PADDING);
  ImGui::SetCursorPosY(0);
  elements::VerticalSeparator(container_height, 8);
  ImGui::SameLine(ICON_SIZE + ICON_PADDING * 2, 8);

  ImVec2 content_size_right_to_separator = ImGui::GetContentRegionAvail();
  float content_box_width =
    content_size_right_to_separator.x - ICON_SIZE * 2 - ICON_PADDING * 3;
  float content_box_height = container_height - 8.0f;
  float content_box_y_offset = 8;
  {
    ImGui::SetCursorPos(
      ImVec2(ImGui::GetCursorPosX() + 12, content_box_y_offset));

    ImGui::BeginChild(
      std::format("EventContainerTexts_{}", std::to_string(event.id)).c_str(),
      ImVec2(content_box_width, content_box_height), false,
      ImGuiWindowFlags_NoScrollbar);

    RenderDateInput(event, content_box_width);
    ImGui::Spacing();

    RenderHeadlineInput(event, content_box_width);
    ImGui::Spacing();

    if (event.expanded) {
      RenderDescriptionInput(event, content_box_width, order);
      ImGui::Spacing();
    }
    RenderExpanderButton(event, content_box_width, container_height);
    ImGui::EndChild();
  }

  ImGui::SameLine(
    content_size_right_to_separator.x - ICON_SIZE * 2 + ICON_PADDING * 3, 8);
  ImGui::SetCursorPosY(0);
  elements::VerticalSeparator(container_height, 8);
  ImGui::SameLine(
    content_size_right_to_separator.x - ICON_SIZE * 2 - ICON_PADDING * 4);

  auto name = std::format("##delete_button_{}", event.id);
  elements::RenderIconButton(name, p_delete_icon_, ICON_PADDING,
    container_height, ICON_SIZE,
    ImVec2(
      content_size_right_to_separator.x - ICON_SIZE * 2 + ICON_PADDING * 5 + 8,
      -3),
    20, 36, ICON_SIZE + ICON_PADDING,

    ImVec2(
      content_size_right_to_separator.x - ICON_SIZE * 3 + ICON_PADDING * 6 + 8,
      event.expanded ? content_box_height / 4.5 : content_box_height / 8),
    [this, &event, &document]() { DeleteEvent(document, event); });

  ImGui::EndChild();
  ImGui::PopStyleColor(2);
  ImGui::PopStyleVar(3);
  ImGui::Separator();
}

void UiDocumentTab::RenderExpanderButton(
  TimelineEvent& event, float width, float height) {
  auto icon = event.expanded ? p_arrow_drop_up_icon_ : p_arrow_drop_down_icon_;
  auto icon_pos =
    event.expanded
      ? ImVec2(width / 2 - 16, height - ICON_SIZE - ICON_PADDING + 2)
      : ImVec2(width / 2 - 16, height - ICON_SIZE - ICON_PADDING);
  auto name = std::format("##expander_button_{}", event.id);
  if (elements::RenderIconButton(name, icon, 3, ICON_SIZE, ICON_SIZE, icon_pos,
        0, 20, width - 16, ImVec2(0, height - 28))) {
    event.expanded = !event.expanded;
  }
}

void UiDocumentTab::RenderDateInput(TimelineEvent& event, const float width) {
  ImGui::Text("Date");
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
  ImGui::SetNextItemWidth(width - 72);
  year_ = event.year;
  if (year_ < 0) {
    year_ *= -1;
  }
  if (ImGui::DragInt(
        "##Date", &year_, 1, 0, 100000, "%d", ImGuiSliderFlags_AlwaysClamp)) {
    event.year = year_;
    ParseYear(event, index_bc_ac_);
  }
  if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  ImGui::SameLine(width - 60, 0);
  ImGui::SetNextItemWidth(45);
  if (ImGui::BeginCombo(std::format("##BC_AC_{}", event.id).c_str(),
        bc_ac_items_[index_bc_ac_], ImGuiComboFlags_NoArrowButton)) {
    for (int n = 0; n < IM_ARRAYSIZE(bc_ac_items_); n++) {
      const bool is_selected = (index_bc_ac_ == n);
      if (ImGui::Selectable(bc_ac_items_[n], is_selected)) {
        index_bc_ac_ = n;
        ParseYear(event, index_bc_ac_);
      }

      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  ImGui::PopStyleVar(2);
}

void UiDocumentTab::RenderHeadlineInput(
  TimelineEvent& event, const float width) {
  ImGui::Text("Headline");
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
  ImGui::SetNextItemWidth(width - 16);
  strncpy(a_buffer_headline_, event.headline.c_str(), BUFFER_HEADLINE_SIZE);
  if (ImGui::InputText(std::format("##HeadlineInput_{}", event.id).c_str(),
        a_buffer_headline_, BUFFER_HEADLINE_SIZE)) {
    event.headline = std::string(a_buffer_headline_);
  }
  ImGui::PopStyleVar(2);
}

void UiDocumentTab::RenderDescriptionInput(
  TimelineEvent& event, float width, uint64_t order) {
  ImGui::Text("Description");
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
  ImGui::SetNextItemWidth(width - 16);
  strncpy(
    a_buffer_description_, event.description.c_str(), BUFFER_DESCRIPTION_SIZE);
  a_buffer_description_[BUFFER_DESCRIPTION_SIZE - 1] = '\0';
  if (ImGui::InputText(std::format("##DescriptionInput_{}", event.id).c_str(),
        a_buffer_description_, BUFFER_DESCRIPTION_SIZE)) {
    event.description = std::string(a_buffer_description_);
  }
  ImGui::PopStyleVar(2);
}

void UiDocumentTab::ParseYear(TimelineEvent& event, uint64_t index) {
  if (event.year < 0 && index == kAC) {
    event.year = -event.year;
  }
  if (event.year > 0 && index == kBC) {
    event.year = -event.year;
  }
}

void UiDocumentTab::DeleteEvent(Document& doc, const TimelineEvent& event) {
  for (uint64_t i = 0; i < doc.events.size(); ++i) {
    if (doc.events[i].id == event.id) {
      doc.events.erase(doc.events.begin() + i);
    }
  }
}

void UiDocumentTab::SwapEvents(
  Document& document, uint64_t source_index, uint64_t target_index) {
  if (source_index != target_index && source_index >= 0 &&
      source_index < document.events.size() && target_index >= 0 &&
      target_index < document.events.size()) {
    std::swap(document.events[source_index], document.events[target_index]);
  }
}

}  // namespace linea_one::ui
