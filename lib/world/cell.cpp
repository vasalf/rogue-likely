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

#include <world/cell.h>

namespace NRogueLikely {
namespace {

class TEmptyCell final : public TCellBase {
public:
    TEmptyCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::EMPTY;
    }
};

class TFloorCell final : public TCellBase {
public:
    TFloorCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::FLOOR;
    }
};

class TPassageCell final : public TCellBase {
public:
    TPassageCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::PASSAGE;
    }
};

class TWallCell final : public TCellBase {
public:
    TWallCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::WALL;
    }
};

} // namespace <anonymous>

TCellPtr MakeCell(ECellFloor cellFloor) {
    switch (cellFloor) {
        case ECellFloor::EMPTY:
            return std::make_shared<TEmptyCell>();
        case ECellFloor::FLOOR:
            return std::make_shared<TFloorCell>();
        case ECellFloor::WALL:
            return std::make_shared<TWallCell>();
        case ECellFloor::PASSAGE:
            return std::make_shared<TPassageCell>();
    }
}

} // namespace NRogueLikely
