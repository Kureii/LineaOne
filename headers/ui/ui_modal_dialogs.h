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
 * File: ui_modal_dialogs.h
 * Created by kureii on 8/15/24
 */
#pragma once

#include <document_manager.h>
#include <export_document.h>
#include <memory>

namespace linea_one::ui {

class UiModalDialogs {
 public:
  explicit UiModalDialogs(const std::shared_ptr<DocumentManager> &p_doc_man);

  void RenderUnsavedChanges();
  void RenderSaveDialog();
  void RenderLoadDialog();
  void RenderExportDialog();
  void RefreshDirectoryContents();

  void SetShowUnsavedDialog(const bool show_unsaved_dialog);
  void SetShowSaveDialog(const bool show_save_dialog);
  void SetShowLoadDialog(const bool show_load_dialog);
  void SetShowExportDialog(const bool show_export_dialog);
  [[nodiscard]] bool GetShowUnsavedDialog() const;
  [[nodiscard]] bool GetShowSaveDialog() const;
  [[nodiscard]] bool GetShowLoadDialog() const;
  [[nodiscard]] bool GetShowExportDialog() const;


 private:
  std::shared_ptr<DocumentManager> p_doc_man_;
  bool show_unsaved_dialog_;
  bool show_save_dialog_;
  bool show_load_dialog_ = false;
  bool show_export_dialog_ = false;
  std::filesystem::path current_path_;
 std::optional<std::filesystem::path> selected_file_;
  std::vector<std::string> dir_contents_;
  std::string file_name_ = "Untitled.jsonlo";
  char file_name_buffer_[256];
  int selected_index_ = -1;
  ExportDocument export_doc_;
};

}  // namespace linea_one::ui