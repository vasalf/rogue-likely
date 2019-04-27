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
#include <object/object.h>
#include <ui/user_action.h>

#include <memory>

namespace NRogueLikely {

class TAbstractUI {
public:
    TAbstractUI() = default;

    TAbstractUI(const TAbstractUI&) = delete;
    TAbstractUI& operator=(const TAbstractUI&) = delete;
    TAbstractUI(TAbstractUI&&) = default;
    TAbstractUI& operator=(TAbstractUI&&) = default;

    virtual void DrawMap(const TWorld& world, TObjectPtr center) = 0;
    virtual TUserActionPtr AwaitUserAction() = 0;
};

using TAbstractUIPtr = std::shared_ptr<TAbstractUI>;

}