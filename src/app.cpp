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
#include <app.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <format>
#include <iostream>

namespace linea_one {
App::App() : stop_(false), clear_color_(0.45f, 0.55f, 0.6f, 1.0f) {
  p_doc_man_ = std::make_shared<DocumentManager>();
  p_input_man_ = std::make_unique<InputManager>(p_doc_man_);
}

App::~App() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  SDL_Quit();
}

bool App::Init() {
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0) {
    std::cerr << "Error: SDL_Init(): " << SDL_GetError() << std::endl;
    return false;
  }

  if (!CreateWindow()) {
    return false;
  }
  p_renderer_ = std::make_shared<Renderer>(p_window_, p_doc_man_, p_input_man_);
  if (!p_renderer_->Init()) {
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
    stop_ = p_input_man_->HandleEvents(p_window_.get());
    if (p_input_man_->HandleShortcuts() == ASCII_W) {
      p_renderer_->SetShowUnsavedDialog(true);
    }
    Update();
    p_renderer_->Render();

    stop_ |= p_renderer_->GetStopRendering();
  }
}

void App::Update() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

bool App::CreateWindow() {
  // Create window with SDL_Renderer graphics context
  Uint32 window_flags =
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;

  p_window_ = std::shared_ptr<SDL_Window>(
    SDL_CreateWindow("LeneaOne", 1280, 720, window_flags), SDL_DestroyWindow);
  if (p_window_ == nullptr) {
    std::cerr << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
    return false;
  }
  SDL_SetWindowPosition(
    p_window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(p_window_.get());

  SDL_SetWindowMinimumSize(p_window_.get(), 960, 720);

  return true;
}

void App::SetupImGui() const {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
    ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
    ImGuiConfigFlags_NavEnableGamepad;               // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(
    p_window_.get(), p_renderer_->GetSdlRenderer().get());
  ImGui_ImplSDLRenderer3_Init(p_renderer_->GetSdlRenderer().get());

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

}  // namespace linea_one