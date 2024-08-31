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
 * File: ui_document_tab.h
 * Created by kureii on 8/14/24
 */
#pragma once
#include <document.h>
#include <document_manager.h>
#include <svg_icon.h>
#include <ui/ui_draw_timeline.h>

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

#define EVENT_CONTAINER_HEIGHT 105
#define EVENT_CONTAINER_HEIGHT_EXPANDED 139
#define DRAG_INDICATOR_ICON_PATH RESOURCES_PATH "/icons/drag_indicator.svg"
#define DELETE_FOREVER_ICON_PATH RESOURCES_PATH "/icons/delete_forever.svg"
#define ARROW_DROP_UP_ICON_PATH RESOURCES_PATH "/icons/arrow_drop_up.svg"
#define ARROW_DROP_DOWN_ICON_PATH RESOURCES_PATH "/icons/arrow_drop_down.svg"
#define ICON_SIZE 24.0f
#define ICON_PADDING 8.0f
#define BUFFER_HEADLINE_SIZE 256
#define BUFFER_DESCRIPTION_SIZE 2048
#define MIN_SIZE_LEFT_PANEL 400.0f

namespace linea_one::ui {

enum e_dating { kBC = 0, kAC };

class UiDocumentTab {
 public:
  explicit UiDocumentTab(const std::shared_ptr<SDL_Renderer>& p_renderer,
    std::shared_ptr<DocumentManager> p_doc_man);
  ~UiDocumentTab();
  void Render(Document& document, uint64_t index);
  void AddNewEvent(Document& document);
  void StartSort(Document& document, uint64_t index,
    const std::function<void(Document& document, uint64_t index)>& callback);
  [[nodiscard]] bool IsSorting();

 private:
  inline void RenderLeftBox(Document& document, uint64_t index);
  inline void RenderRightBox(Document& document);
  inline void RenderEventBox(
    Document& document, TimelineEvent& event, uint64_t order);
  inline void RenderExpanderButton(
    TimelineEvent& event, float width, float height);
  inline void RenderDateInput(TimelineEvent& event, float width);
  inline void RenderHeadlineInput(TimelineEvent& event, float width);
  inline void RenderDescriptionInput(
    TimelineEvent& event, float width, uint64_t order);
  inline void ParseYear(TimelineEvent& event, uint64_t index);
  inline void DeleteEvent(Document& doc, const TimelineEvent& event);
  inline void SwapEvents(
    Document& document, uint64_t source_index, uint64_t target_index);
  inline void RenderSort(Document& document, uint64_t index,
    ImVec2 content_size);
  inline void DocumentHasChanged();

  std::shared_ptr<SDL_Renderer> p_renderer_;
  std::shared_ptr<svg::SvgIcon> p_drag_icon_;
  std::shared_ptr<svg::SvgIcon> p_delete_icon_;
  std::shared_ptr<svg::SvgIcon> p_arrow_drop_up_icon_;
  std::shared_ptr<svg::SvgIcon> p_arrow_drop_down_icon_;
  std::shared_ptr<DocumentManager> p_doc_man_;
  char* a_buffer_headline_;
  char* a_buffer_description_;
  int index_bc_ac_ = kAC;
  const char* bc_ac_items_[2] = {"BC", "AC"};
  int year_, new_year_;
  uint64_t last_id_ = 0;
  float left_panel_width_ = MIN_SIZE_LEFT_PANEL;
  std::atomic<bool> is_sorting_{false};
  std::jthread sorting_thread_;
};

}  // namespace linea_one::ui
