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

#include <object/object_fwd.h>
#include <world/cell.h>
#include <world/position.h>
#include <world/world_view.h>

#include <string>
#include <vector>

#pragma once

namespace NRogueLikely {

class TWorld {
public:
    TWorld(int levels, int height, int width);

    TWorld(const TWorld&) = delete;

    TWorld& operator=(const TWorld&) = delete;

    TWorld(TWorld&&) noexcept = default;

    TWorld& operator=(TWorld&&) noexcept = default;

    void SetCell(int level, int i, int j, TCellPtr cellPtr);

    void SetCell(const TPosition& position, TCellPtr cellPtr);

    TCellPtr GetCell(int level, int i, int j);

    TCellPtr GetCell(const TPosition& position);

    int GetLevelsCount() const;

    int GetLevelHeight() const;

    int GetLevelWidth() const;

    TWorldView GetViewOf(TObjectPtr object) const;

    void SaveToFile(const std::string& filename);

    void AddObject(const TObjectPtr& object);

    bool TryToMoveObject(const TObjectPtr& object, TPosition newPosition);

private:
    int Levels_;
    int Height_;
    int Width_;

    using TLevel = std::vector<std::vector<TCellPtr>>;
    std::vector<TLevel> World_;
};

}
