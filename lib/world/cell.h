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

#include <object/object_fwd.h>
#include <world/position.h>

#include <memory>
#include <vector>

namespace NRogueLikely {

enum class ECellFloor {
    EMPTY,
    FLOOR,
    PASSAGE,
    WALL,
    LADDER_UP,
    LADDER_DOWN
};

class TCellBase {
public:
    TCellBase() = default;

    TCellBase(const TCellBase&) = delete;
    TCellBase& operator=(const TCellBase&) = delete;
    TCellBase(TCellBase&&) noexcept = default;
    TCellBase& operator=(TCellBase&&) noexcept = default;

    virtual ECellFloor GetFloorType() = 0;
    virtual bool CanMoveHere() const = 0;
    virtual bool CanMoveFromHere(TPosition here, TPosition where) const = 0;

    void AddObject(TObjectPtr object);
    void RemoveObject(TObjectPtr object);
    const std::vector<TObjectPtr>& GetObjectsHere() const;
private:
    std::vector<TObjectPtr> ObjectsHere_;
};

using TCellPtr = std::shared_ptr<TCellBase>;

TCellPtr MakeCell(ECellFloor floorType);

}