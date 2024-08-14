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
 * File: renderer.h
 * Created by kureii on 8/14/24
 */
#pragma once

#include <SDL3/SDL.h>
#include <document.h>
#include <document_manager.h>

#include <memory>

namespace linea_one {

class Renderer {
public:
 Renderer(const std::shared_ptr<SDL_Window>& p_window,
          const std::shared_ptr<DocumentManager>& doc_man);
 ~Renderer();
 bool Init();
 bool Render();
 void SetShowUnsavedDialog(const bool show_unsaved_dialog);
[[nodiscard]] std::shared_ptr<SDL_Renderer> GetSdlRenderer();

private:
 bool RenderMenu();
 void RenderContent();
 void RenderTabs();
 void RenderTabContent(const Document& doc);
 void RenderUnsavedChangesDialog();
 std::shared_ptr<SDL_Window> p_window_;
 std::shared_ptr<SDL_Renderer> p_renderer_;
 std::shared_ptr<DocumentManager> doc_man_;
 bool show_unsaved_dialog_ = false;

};

} // linea_one
