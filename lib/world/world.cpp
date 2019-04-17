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

#include <world/world.h>
#include <object/object.h>

#include <fstream>
#include <utility>

namespace NRogueLikely {

TWorld::TWorld(int levels, int height, int width)
    : Levels_(levels)
    , Height_(height)
    , Width_(width)
{
    World_.resize(levels);
    for (int i = 0; i < levels; i++) {
        World_[i].resize(height);
        for (int j = 0; j < height; j++) {
            World_[i][j].resize(width);
            for (int k = 0; k < width; k++) {
                World_[i][j][k] = MakeCell(ECellFloor::EMPTY);
            }
        }
    }
}

void TWorld::SetCell(int level, int i, int j, TCellPtr cellPtr)  {
    World_[level][i][j] = std::move(cellPtr);
}

void TWorld::SetCell(const TPosition& position, TCellPtr cellPtr) {
    SetCell(position.Level, position.I, position.J, std::move(cellPtr));
}

TCellPtr TWorld::GetCell(int level, int i, int j) const {
    return World_[level][i][j];
}

TCellPtr TWorld::GetCell(const TPosition& position) const {
    return GetCell(position.Level, position.I, position.J);
}

int TWorld::GetLevelsCount() const {
    return Levels_;
}

int TWorld::GetLevelHeight() const {
    return Height_;
}

int TWorld::GetLevelWidth() const {
    return Width_;
}

TWorldView TWorld::GetViewOf(TObjectPtr objectPtr) const {
    int level = objectPtr->GetPosition().value().Level;
    TWorldView ret(Height_, Width_);
    for (int i = 0; i < Height_; i++) {
        for (int j = 0; j < Width_; j++) {
            ret.SetCell(i, j, World_[level][i][j]);
        }
    }
    return ret;
}

void TWorld::SaveToFile(const std::string& filename) {
    std::ofstream file(filename);
    file << Levels_ << " " << Height_ << " " << Width_ << std::endl;
    file << StartPosition_.Level << " " << StartPosition_.I << " " << StartPosition_.J << std::endl;
    for (int i = 0; i < Levels_; i++) {
        for (int j = 0; j < Height_; j++) {
            for (int k = 0; k < Width_; k++) {
                file << (int)GetCell(i, j, k)->GetFloorType() << " ";
            }
        }
    }
}

void TWorld::AddObject(const TObjectPtr& object) {
    auto position = object->GetPosition();
    if (position.has_value()) {
        GetCell(position.value())->AddObject(object);
    }
}

namespace {

bool CheckPositionBounds(TPosition position, int levels, int height, int width) {
    return 0 <= position.Level && position.Level < levels
            && 0 <= position.I && position.I < height
            && 0 <= position.J && position.J < width;
}

}

bool TWorld::TryToMoveObject(const TObjectPtr& object, TPosition newPosition) {
    if (!CheckPositionBounds(newPosition, Levels_, Height_, Width_)) {
        return false;
    }
    if (GetCell(newPosition)->CanMoveHere()) {
        auto oldPosition = object->GetPosition();
        if (oldPosition.has_value()) {
            if (!GetCell(oldPosition.value())->CanMoveFromHere(oldPosition.value(), newPosition)) {
                return false;
            }
            GetCell(oldPosition.value())->RemoveObject(object);
        }
        GetCell(newPosition)->AddObject(object);
        object->SetPosition(newPosition);
        return true;
    }
    return false;
}

void TWorld::SetStartPosition(TPosition position) {
    StartPosition_ = position;
}

TPosition TWorld::GetStartPosition() const {
    return StartPosition_;
}

}
