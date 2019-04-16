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

#include <memory>

namespace NRogueLikely {

class IWorldGenerator {
public:
    IWorldGenerator() = default;

    IWorldGenerator(const IWorldGenerator&) = delete;
    IWorldGenerator& operator=(const IWorldGenerator&) = delete;
    IWorldGenerator(IWorldGenerator&&) noexcept = default;
    IWorldGenerator& operator=(IWorldGenerator&&) noexcept = default;

    virtual TWorld Generate() = 0;
};

using TWorldGeneratorPtr = std::shared_ptr<IWorldGenerator>;

class TPlainWorldGenerator : public IWorldGenerator {
public:
    TPlainWorldGenerator(int levels, int height, int width);

    virtual TWorld Generate() override;

private:
    int Levels_;
    int Height_;
    int Width_;
};

class TLoadWorldGenerator final : public IWorldGenerator {
public:
    TLoadWorldGenerator(const std::string& filename);

    TWorld Generate() override;

private:
    std::string Filename_;
};

}
