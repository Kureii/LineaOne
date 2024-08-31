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
#include <config.h>
#include <document_manager.h>

#include <algorithm>
#include <format>
#include <fstream>

namespace linea_one {

DocumentManager::DocumentManager() {
  documents_ = std::vector<Document>();
}

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

void DocumentManager::SaveDocument() {
  if (current_document_ < 0 || documents_[current_document_].saved || documents_[current_document_].path.empty()) {
    return;
  }
  std::ofstream file( documents_[current_document_].path, std::ios::binary);
  if (file.is_open()) {
    std::string serializedData = SerializeDocument(documents_[current_document_]);
    file.write(serializedData.c_str(), serializedData.size());
    file.close();

    if (file.good()) {
      documents_[current_document_].saved = true;
    }
  }
}

void DocumentManager::LoadDocument(std::filesystem::path path) {
  std::ifstream file(path, std::ios::binary);
  if (file.is_open()) {
    auto doc = DeserializeDocument(file);
    doc.path = path;
    documents_.emplace_back(doc);
    file.close();
  }
}

std::string DocumentManager::SerializeDocument(Document& document) {
  auto json_string = std::format(R"({{
  "Name": "{}",
  "Version": "{}.{}",
  "State": {{
    "Zoom": {},
    "Offset": {}
  }},
  "Events": [
)",
    document.name, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR,
    document.state.zoom, document.state.offset);

  for (uint64_t i = 0; i < document.events.size(); i++) {
    json_string += std::format(R"(    {{
      "Id": {},
      "Year": {},
      "Headline": "{}",
      "Description": "{}",
      "Expanded": {}
    }})",document.events[i].id, document.events[i].year,
    document.events[i].headline, document.events[i].description,
    document.events[i].expanded);
    if (i+1 != document.events.size()) {
      json_string += ",\n";
    } else {
      json_string += "\n";
    }
  }
  json_string += "  ]\n}\n";
  return json_string;
}

Document DocumentManager::DeserializeDocument(
  std::ifstream &json_file) {
    nlohmann::json json_data = nlohmann::json::parse(json_file);
  Document document;
  document.name = json_data["Name"];
  document.saved = true;
  document.state.offset = json_data["State"]["Offset"];
  document.state.zoom = json_data["State"]["Zoom"];
  for (uint64_t i = 0; i < json_data["Events"].size(); i++) {
    TimelineEvent event;
    event.id = json_data["Events"][i]["Id"];
    event.year = json_data["Events"][i]["Year"];
    event.headline = json_data["Events"][i]["Headline"];
    event.description = json_data["Events"][i]["Description"];
    event.expanded = json_data["Events"][i]["Expanded"];
    document.events.emplace_back(event);
  }
  return document;
}

}  // namespace linea_one