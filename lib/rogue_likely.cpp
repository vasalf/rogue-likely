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

#include <rogue_likely.h>
#include <ui/abstract_ui.h>
#include <ui/ncurses/ncurses_ui.h>

namespace NRogueLikely {

void RunMain() {
    TWorld world(3, 10, 10);
    world.SetCell(0, 0, 0, MakeCell(ECellFloor::FLOOR));
    world.SetCell(0, 0, 1, MakeCell(ECellFloor::FLOOR));
    world.SetCell(0, 9, 9, MakeCell(ECellFloor::WALL));
    TObjectPtr person = std::make_shared<TObject>();
    person->SetPosition({0, 0, 0});
    TAbstractUIPtr ui = std::make_shared<TNCursesUI>();
    ui->DrawMap(world, person);
    ui->AwaitUserAction();
}

}