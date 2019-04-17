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

#include <controller/controller.h>
#include <ui/ncurses/ncurses_ui.h>
#include <world/world_generator.h>

namespace NRogueLikely {
namespace {

TWorld GenerateWorld(const TGameOpts& opts) {
    TWorldGeneratorPtr generatorPtr;
    if (opts.MapLoadPath.has_value()) {
        generatorPtr = std::make_shared<TLoadWorldGenerator>(opts.MapLoadPath.value());
    } else {
        generatorPtr = std::make_shared<TPlainWorldGenerator>(opts.Levels, opts.Height, opts.Width);
    }
    return generatorPtr->Generate();
}

}

TController::TController(const TGameOpts& opts)
    : world(GenerateWorld(opts))
    , player(std::make_shared<TAliveObject>(EObjectType::PLAYER, world.GetStartPosition()))
{
    world.AddObject(player);
}

void TController::MainLoop() {
    TAbstractUIPtr ui = std::make_shared<TNCursesUI>();
    while (true) {
        try {
            TUserActionPtr userAction;
            do {
                ui->DrawMap(world, player);
                userAction = ui->AwaitUserAction();
            } while (!userAction->Perform(world, player));
        } catch(TExitGameException&) {
            break;
        }
    }
}

}
