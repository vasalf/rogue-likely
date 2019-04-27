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

#include <object/alive_object.h>

namespace NRogueLikely {

TAliveObjectEmptyPosition::TAliveObjectEmptyPosition()
    : std::runtime_error("Attempt to set empty position to an alive object")
{}

TAliveObject::TAliveObject(EObjectType objectType, TPosition position)
    : TObject(objectType)
{
    SetPosition(position);
}

void TAliveObject::SetEmptyPosition() {
    throw TAliveObjectEmptyPosition();
}

TPosition TAliveObject::GetPositionSafe() const {
    return TObject::GetPosition().value();
}

}