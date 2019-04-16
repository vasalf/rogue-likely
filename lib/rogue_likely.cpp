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
#include <object/alive_object.h>
#include <ui/abstract_ui.h>
#include <ui/ncurses/ncurses_ui.h>
#include <world/world_generator.h>

#include <CLI/CLI11.hpp>

namespace NRogueLikely {

int RunMain(int argc, char* argv[]) {
    std::string mapFilename;
    CLI::App app;
    app.add_option("-l,--load-map", mapFilename, "Load map from file");
    CLI11_PARSE(app, argc, argv);

    TWorldGeneratorPtr worldGenerator;
    if (mapFilename.empty()) {
        worldGenerator = std::make_shared<TPlainWorldGenerator>(3, 10, 10);
    } else {
        worldGenerator = std::make_shared<TLoadWorldGenerator>(mapFilename);
    }

    TWorld world = worldGenerator->Generate();
    TAliveObjectPtr person = std::make_shared<TAliveObject>(EObjectType::PLAYER, TPosition(0, 4, 4));
    world.AddObject(person);
    TAbstractUIPtr ui = std::make_shared<TNCursesUI>();
    while (true) {
        ui->DrawMap(world, person);
        auto action = ui->AwaitUserAction();
        action->Perform(world, person);
    }
}

}