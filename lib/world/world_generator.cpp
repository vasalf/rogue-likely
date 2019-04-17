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

#include <world/world_generator.h>

#include <fstream>

namespace NRogueLikely {

TPlainWorldGenerator::TPlainWorldGenerator(int levels, int height, int width)
    : Levels_(levels)
    , Height_(height)
    , Width_(width)
{}

TWorld TPlainWorldGenerator::Generate() {
    TWorld ret(Levels_, Height_, Width_);
    for (int i = 0; i < Levels_; i++) {
        for (int k = 0; k < Width_; k++) {
            ret.SetCell(i, 0, k, MakeCell(ECellFloor::WALL));
            ret.SetCell(i, Height_ - 1, k, MakeCell(ECellFloor::WALL));
        }
        for (int j = 1; j < Height_ - 1; j++) {
            ret.SetCell(i, j, 0, MakeCell(ECellFloor::WALL));
            for (int k = 1; k < Width_ - 1; k++) {
                ret.SetCell(i, j, k, MakeCell(ECellFloor::FLOOR));
            }
            ret.SetCell(i, j, Width_ - 1, MakeCell(ECellFloor::WALL));
        }
        if (i > 0) {
            ret.SetCell(i, 1, 1, MakeCell(ECellFloor::LADDER_DOWN));
        }
        if (i < Levels_ - 1) {
            ret.SetCell(i, 1, 2, MakeCell(ECellFloor::LADDER_UP));
        }
    }
    ret.SetStartPosition(TPosition(0, Height_ / 2, Width_ / 2));
    return ret;
}

TLoadWorldGenerator::TLoadWorldGenerator(const std::string& filename)
    : Filename_(filename)
{ }

TWorld TLoadWorldGenerator::Generate() {
    std::ifstream file(Filename_);
    int levels, height, width;
    file >> levels >> height >> width;
    TPosition startPosition;
    file >> startPosition.Level >> startPosition.I >> startPosition.J;
    TWorld world(levels, height, width);
    world.SetStartPosition(startPosition);
    for (int i = 0; i < levels; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                int x;
                file >> x;
                auto floor = (ECellFloor)x;
                world.SetCell(i, j, k, MakeCell(floor));
            }
        }
    }
    return world;
}

}
