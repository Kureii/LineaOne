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
 * File: document_manager.h
 * Created by kureii on 8/11/24
 */
#pragma once

#include <document.h>


#include <cstdint>
#include <vector>
#include <nlohmann/json.hpp>

namespace linea_one {

class DocumentManager {
 public:
  DocumentManager();
  void CreateNewDocument();
  /*Never call this function before "CloseDocumentWithCheck" */
  void CloseDocument();
  int32_t CloseDocumentWithCheck(int32_t const index);
  Document* GetCurrentDocument();
  [[nodiscard]] int32_t DocumentSize() const;
  [[nodiscard]] Document& GetSpecificDocument(int32_t const index);
  [[nodiscard]] int32_t GetCurrentDocumentIndex() const;
  [[nodiscard]] int32_t GetDocToClose() const;
  void SetCurrentDocumentIndex(int32_t const index);
  void SetDocToClose(int32_t const index);
  void SetDocOnIndex(Document& document, int64_t const index);
  void SaveDocument();
  void LoadDocument(std::filesystem::path path);

 private:
  std::string SerializeDocument(Document &document);
  Document DeserializeDocument(std::ifstream &json_file);
  std::vector<Document> documents_;
  uint64_t new_doc_counter = 0;
  int32_t current_document_ = -1;
  int32_t doc_to_close_ = -1;
};

}  // namespace linea_one
