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
 * File: ui_manager.h
 * Created by kureii on 8/14/24
 */
#pragma once
#include <document.h>
#include <document_manager.h>
#include <input_manager.h>
#include <ui/ui_document_tab.h>
#include <ui/ui_main_menu.h>

#include <memory>

#include "ui_modal_dialogs.h"

namespace linea_one::ui {

class UiManager {
 public:
  UiManager(const std::shared_ptr<DocumentManager>& p_doc_man,
    const std::shared_ptr<SDL_Renderer>& p_renderer,
    const std::shared_ptr<InputManager>& p_input_man);
  void RenderMenu();
  void RenderContent();
  void RenderTabs();
  void RenderTabContent(Document& doc) const;
  void SetShowUnsavedDialog(const bool show_unsaved_dialog);
  [[nodiscard]] bool GetStopRendering() const;
  void SetStopRendering(const bool stop_rendering);
  void SetSharedVars() const;
  [[nodiscard]] std::shared_ptr<UiDocumentTab> GetUiDocumentTab() const;

 private:
  std::shared_ptr<DocumentManager> p_doc_man_;
  std::unique_ptr<UiMainMenu> p_main_menu_;
  std::shared_ptr<UiDocumentTab> p_doc_tab_;
  std::unique_ptr<UiModalDialogs> p_modal_dialogs_;
  std::shared_ptr<SDL_Renderer> p_renderer_;
  std::shared_ptr<InputManager> p_input_man_;
  bool show_unsaved_dialog_ = false;
  bool stop_rendering_ = false;
  bool new_tab_request_ = false;
};

}  // namespace linea_one::ui
