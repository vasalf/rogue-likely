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

#include <world/position.h>

#include <tuple>

namespace NRogueLikely {

TPosition::TPosition(int level, int i, int j)
    : Level(level)
    , I(i)
    , J(j)
{ }

bool operator<(const TPosition& a, const TPosition& b) {
    return std::make_tuple(a.Level, a.I, a.J) < std::make_tuple(b.Level, b.I, b.J);
}

bool operator!=(const TPosition& a, const TPosition& b) {
    return std::make_tuple(a.Level, a.I, a.J) != std::make_tuple(b.Level, b.I, b.J);
}

}
