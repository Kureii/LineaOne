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

#include <imgui.h>

namespace linea_one::ui {

UiDocumentTab::UiDocumentTab() {}

void UiDocumentTab::Render(Document& document) {
  ImGui::Text("Content of document: %s", document.name.c_str());
}


} // linea_one::ui