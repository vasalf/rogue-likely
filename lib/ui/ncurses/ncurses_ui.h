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

#include <ui/abstract_ui.h>
#include <ui/user_action.h>

namespace NRogueLikely {

class TNCursesUI final : public TAbstractUI {
public:
    TNCursesUI();
    ~TNCursesUI();

    void DrawMap(const TWorld& world, TObjectPtr objectPtr) override;
    TUserActionPtr AwaitUserAction() override;

private:
    class TImpl;
    std::unique_ptr<TImpl> impl_;
};

}

