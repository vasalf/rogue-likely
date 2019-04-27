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
#include <world/world.h>

namespace NRogueLikely {

class IUserAction {
public:
    IUserAction() = default;

    IUserAction(const IUserAction&) = delete;
    IUserAction& operator=(const IUserAction&) = delete;
    IUserAction(IUserAction&&) noexcept = default;
    IUserAction& operator=(IUserAction&&) noexcept = default;

    virtual bool Perform(TWorld& world, TAliveObjectPtr object) = 0;
};

using TUserActionPtr = std::shared_ptr<IUserAction>;

}

