/**
 * Copyright 2019 Grigory Bartosh, Pavel Solikov, Vasily Alferov
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
 */

#pragma once

#include <world/world.h>
#include <object/alive_object.h>

#include <optional>
#include <string>

namespace NRogueLikely {

class TExitGameException : public std::exception {};

struct TGameOpts {
    int Levels = 0;
    int Height = 0;
    int Width = 0;
    std::optional<std::string> MapLoadPath;
};

class TController {
public:
    TController(const TGameOpts& opts);

    TController(const TController&) = delete;
    TController& operator=(const TController&) = delete;
    TController(TController&&) noexcept = default;
    TController& operator=(TController&&) noexcept = default;

    void MainLoop();

private:
    TWorld world;
    TAliveObjectPtr player;
};

}