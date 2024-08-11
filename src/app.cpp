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
 * File: app.cpp
 * Created by kureii on 8/11/24
 */
#include "app.h"

#include <format>
#include <iostream>

#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace linea_one {
App::App()
    : p_window_(nullptr),
      p_renderer_(nullptr),
      stop_(false),
      clear_color_(0.45f, 0.55f, 0.6f, 1.0f) {
  doc_man_ = std::make_shared<DocumentManager>();
  input_man_ = std::make_unique<InputManager>(doc_man_);
}

App::~App() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(p_renderer_);
  SDL_DestroyWindow(p_window_);
  SDL_Quit();
}

bool App::Init() {
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0) {
    std::cout << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
    return false;
  }

  if (!CreateWindow()) {
    return false;
  }

  if (!CreateRenderer()) {
    return false;
  }

  SetupImGui();
  return true;
}

void App::Run() {
#ifdef __EMSCRIPTEN__
  // For an Emscripten build we are disabling file-system access, so let's not
  // attempt to do a fopen() of the imgui.ini file. You may manually call
  // LoadIniSettingsFromMemory() to load settings from your own storage.
  io.IniFilename = nullptr;
  EMSCRIPTEN_MAINLOOP_BEGIN
#else
  while (!stop_)
#endif
  {
    stop_ = input_man_->HandleEvents(p_window_);
    if (input_man_->HandleShortcuts() == ASCII_W) {
      show_unsaved_dialog_ = true;
    }
    Update();
    Render();
  }
}

void App::Update() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void App::Render() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("Fullscreen Window", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_MenuBar);

  RenderMenu();
  RenderContent();

  ImGui::End();

  ImGui::Render();
  SDL_SetRenderDrawColor(p_renderer_, 0, 0, 0, 255);
  SDL_RenderClear(p_renderer_);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),
                                        p_renderer_);
  SDL_RenderPresent(p_renderer_);
}

void App::RenderMenu() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New file", "Ctrl+N")) {
        doc_man_->CreateNewDocument();
      }
      if (ImGui::MenuItem("Open file")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Save")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Save as")) { /* TODO: Implement */
      }
      if (ImGui::MenuItem("Close file", "Ctrl+W")) {
        if (doc_man_->CloseDocumentWithCheck(doc_man_->GetCurrentDocumentIndex()) >= 0) {
          show_unsaved_dialog_ = true;
        }
      }
      if (ImGui::MenuItem("Exit")) {
        stop_ = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

void App::RenderContent() {
  RenderTabs();
  if (auto current_document = doc_man_->GetCurrentDocument()) {
    RenderTabContent(*current_document);
  }

  if (show_unsaved_dialog_) {
    RenderUnsavedChangesDialog();
  }
}

bool App::CreateWindow() {
  // Create window with SDL_Renderer graphics context
  Uint32 window_flags =
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
  p_window_ = SDL_CreateWindow("LeneaOne",
                                     1280, 720, window_flags);
  if (p_window_ == nullptr) {
    std::cout << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
    return false;
  }
  SDL_SetWindowPosition(p_window_, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(p_window_);
  return true;
}

bool App::CreateRenderer() {
  p_renderer_= SDL_CreateRenderer(p_window_, nullptr);
  SDL_SetRenderVSync(p_renderer_, 1);
  if (p_renderer_ == nullptr) {
    SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void App::SetupImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;             // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(p_window_, p_renderer_);
  ImGui_ImplSDLRenderer3_Init(p_renderer_);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // - Our Emscripten build process allows embedding fonts to be accessible at
  // runtime from the "fonts/" folder. See Makefile.emscripten for details.
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // nullptr, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr);
}

void App::RenderTabs() {
  if (ImGui::BeginTabBar("DocumentTabs", ImGuiTabBarFlags_AutoSelectNewTabs)) {
    for (uint32_t i = 0; i < doc_man_->DocumentSize(); ++i) {
      bool open = true;
      if (ImGui::BeginTabItem(doc_man_->GetSpecificDocument(i).name.c_str(), &open, ImGuiTabItemFlags_None)) {
        doc_man_->SetCurrentDocumentIndex(i);
        ImGui::EndTabItem();
      }
      if (!open) {
        auto index_to_close = doc_man_->CloseDocumentWithCheck(i);
        if (index_to_close == -1) {
          i--;
        }
        if (index_to_close > -1) {
          show_unsaved_dialog_ = true;
        }
      }
    }
    if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
      doc_man_->CreateNewDocument();
    }
    ImGui::EndTabBar();
  }
}

void App::RenderTabContent(const Document& doc) {
  ImGui::Text("Content of document: %s", doc.name.c_str());
}

void App::RenderUnsavedChangesDialog() {
  ImGui::OpenPopup("Unsaved Changes");

  if (ImGui::BeginPopupModal("Unsaved Changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Document '%s' has unsaved changes. Do you want to save before closing?",
                doc_man_->GetSpecificDocument(doc_man_->GetDocToClose()).name.c_str());
    ImGui::Separator();

    if (ImGui::Button("Save", ImVec2(120, 0))) {
      /* TODO save function */
      doc_man_->GetSpecificDocument(doc_man_->GetDocToClose()).saved = true;
      doc_man_->CloseDocument();
      ImGui::CloseCurrentPopup();
      show_unsaved_dialog_ = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Don't Save", ImVec2(120, 0))) {
      doc_man_->CloseDocument();
      ImGui::CloseCurrentPopup();
      show_unsaved_dialog_ = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
      show_unsaved_dialog_ = false;
    }
    ImGui::EndPopup();
  }
}

}  // namespace linea_ona