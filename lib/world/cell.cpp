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

#include <algorithm>

namespace NRogueLikely {
namespace {

class TEmptyCell final : public TCellBase {
public:
    TEmptyCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::EMPTY;
    }

    bool CanMoveHere() const override {
        return false;
    }

    bool CanMoveFromHere(TPosition here, TPosition where) const override {
        return false;
    }
};

class TFloorCell final : public TCellBase {
public:
    TFloorCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::FLOOR;
    }

    bool CanMoveHere() const override {
        return true;
    }

    bool CanMoveFromHere(TPosition here, TPosition where) const override {
        return here.Level == where.Level;
    }
};

class TPassageCell final : public TCellBase {
public:
    TPassageCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::PASSAGE;
    }

    bool CanMoveHere() const override {
        return true;
    }

    bool CanMoveFromHere(TPosition here, TPosition where) const override {
        return here.Level == where.Level;
    }
};

class TWallCell final : public TCellBase {
public:
    TWallCell() = default;

    ECellFloor GetFloorType() override {
        return ECellFloor::WALL;
    }

    bool CanMoveHere() const override {
        return false;
    }

    bool CanMoveFromHere(TPosition here, TPosition where) const override {
        return false;
    }
};

class TLadderCell final : public TCellBase {
public:
    enum class ELadderType {
        UP,
        DOWN
    };

    TLadderCell(ELadderType ladderType)
        : LadderType_(ladderType)
    {}

    ECellFloor GetFloorType() override {
        if (LadderType_ == ELadderType::UP) {
            return ECellFloor::LADDER_UP;
        } else {
            return ECellFloor::LADDER_DOWN;
        }
    }

    bool CanMoveHere() const override {
        return true;
    }

    bool CanMoveFromHere(TPosition here, TPosition where) const override {
        if (here.Level == where.Level) {
            return true;
        }
        return here.Level == where.Level + (LadderType_ == ELadderType::UP ? -1 : 1)
               && here.I == where.I
               && here.J == where.J;
    }

private:
    ELadderType LadderType_;
};

} // namespace <anonymous>

void TCellBase::AddObject(TObjectPtr object) {
    ObjectsHere_.push_back(object);
}

void TCellBase::RemoveObject(TObjectPtr object) {
    ObjectsHere_.erase(std::find(ObjectsHere_.begin(), ObjectsHere_.end(), object));
}

const std::vector<TObjectPtr>& TCellBase::GetObjectsHere() const {
    return ObjectsHere_;
}

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
        case ECellFloor::LADDER_UP:
            return std::make_shared<TLadderCell>(TLadderCell::ELadderType::UP);
        case ECellFloor::LADDER_DOWN:
            return std::make_shared<TLadderCell>(TLadderCell::ELadderType::DOWN);
    }
}

} // namespace NRogueLikely
