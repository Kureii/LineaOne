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
 * File: input_manager.h
 * Created by kureii on 8/11/24
 */
#pragma once

#include <SDL3/SDL_video.h>
#include <document_manager.h>

#include <cstdint>
#include <memory>

namespace linea_one {

#define ASCII_N 78
#define ASCII_W 87

class InputManager {
public:
 InputManager(std::shared_ptr<DocumentManager> doc_man);
 uint32_t HandleShortcuts();
 bool HandleEvents(SDL_Window* p_window_);

private:
 uint32_t action_key;
 std::shared_ptr<DocumentManager> doc_man_;
};

}
