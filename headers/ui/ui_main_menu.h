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
 * File: ui_main_menu.h
 * Created by kureii on 8/14/24
 */
#pragma once
#include <document_manager.h>

#include <memory>

namespace linea_one::ui {

class UiMainMenu {
 public:
  UiMainMenu(const std::shared_ptr<DocumentManager> &p_doc_man);
  void Render();
  [[nodiscard]] bool IsShowUnsavedDialog() const;
  [[nodiscard]] bool IsShowSaveDialog() const;
  [[nodiscard]] bool IsShowLoadDialog() const;
  [[nodiscard]] bool IsShowExportDialog() const;
  [[nodiscard]] bool IsStopRendering() const;
  void SetShowUnsavedDialog(const bool show_unsaved_dialog);
  void SetShowSaveDialog(const bool show_save_dialog);
  void SetShowLoadDialog(const bool show_save_dialog);
  void SetShowExportDialog(const bool show_save_dialog);

 private:
  void NewFile() const;
  void CloseFile();
  void SaveFileDialog();
  void ExportFileDialog();
  std::shared_ptr<DocumentManager> p_doc_man_;
  bool show_unsaved_dialog_ = false;
  bool show_save_dialog_ = false;
  bool show_load_dialog_ = false;
  bool show_export_dialog_ = false;
  bool stop_rendering_ = false;

};

}  // namespace linea_one::ui
