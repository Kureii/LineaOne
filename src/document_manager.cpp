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
 * File: document_manager.cpp
 * Created by kureii on 8/11/24
 */
#include <document_manager.h>

#include <algorithm>
#include <format>

namespace linea_one {

DocumentManager::DocumentManager() { documents_ = std::vector<Document>(); }

void DocumentManager::CreateNewDocument() {
  new_doc_counter++;
  Document new_doc = {std::format("New Document {}", new_doc_counter), false};
  new_doc.state.zoom = 1.0f;
  new_doc.state.offset = 0.0f;
  new_doc.state.minYear = std::numeric_limits<int>::max();
  new_doc.state.maxYear = std::numeric_limits<int>::min();

  for (const auto& event : new_doc.events) {
    new_doc.state.minYear = std::min(new_doc.state.minYear, event.year);
    new_doc.state.maxYear = std::max(new_doc.state.maxYear, event.year);
  }

  // Ensure we have a valid range even if there are no events
  if (new_doc.state.minYear == std::numeric_limits<int>::max()) {
    new_doc.state.minYear = 2000;
    new_doc.state.maxYear = 2000;
  }

  documents_.push_back(new_doc);
  current_document_ = static_cast<int32_t>(documents_.size() - 1);

}

Document* DocumentManager::GetCurrentDocument() {
  if (current_document_ >= 0 && current_document_ < documents_.size())
    return &documents_[current_document_];
  return nullptr;
}

void DocumentManager::CloseDocument() {
  if (doc_to_close_ >= 0) {
    documents_.erase(documents_.begin() + doc_to_close_);
    if (current_document_ >= documents_.size()) {
      current_document_ =
        static_cast<int>(documents_.empty() ? -1 : documents_.size() - 1);
    }
  }
}

int32_t DocumentManager::CloseDocumentWithCheck(int32_t const index) {
  if (index >= 0 && index < documents_.size()) {
    doc_to_close_ = index;
    if (!documents_[index].saved) {
      return index;
    } else {
      CloseDocument();
      return -1;
    }
  }
  return -2;
}

int32_t DocumentManager::DocumentSize() const {
  return static_cast<int32_t>(documents_.size());
}

Document& DocumentManager::GetSpecificDocument(int32_t const index) {
  return documents_[index];
}

int32_t DocumentManager::GetCurrentDocumentIndex() const {
  return current_document_;
}

int32_t DocumentManager::GetDocToClose() const { return doc_to_close_; }

void DocumentManager::SetCurrentDocumentIndex(int32_t const index) {
  current_document_ = index;
}

void DocumentManager::SetDocToClose(int32_t const index) {
  doc_to_close_ = index;
}

void DocumentManager::SetDocOnIndex(Document& document, int64_t const index) {
  if (index >= 0 && index < documents_.size()) {
    documents_[index] = document;
  }
}


}  // namespace linea_one