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
#include <controller/controller.h>

#include <CLI/CLI11.hpp>

namespace NRogueLikely {

int RunMain(int argc, char* argv[]) {
    TGameOpts opts;
    opts.Levels = 10;
    opts.Height = 50;
    opts.Width = 80;
    opts.LevelGenerationStrategy = "roomed";

    std::string mapFilename;
    CLI::App app;
    app.add_option("-m,--load-map", mapFilename, "Load map from file");
    app.add_option("-L,--levels", opts.Levels, "Number of levels in the game");
    app.add_option("-H,--height", opts.Height, "Height of each level");
    app.add_option("-W,--width", opts.Width, "Width of each level");
    app.add_option("-s,--generation-strategy", opts.LevelGenerationStrategy, "Algorithm to generate the world");
    CLI11_PARSE(app, argc, argv);

    if (!mapFilename.empty()) {
        opts.MapLoadPath = mapFilename;
    }

    TController controller(opts);
    controller.MainLoop();
    return 0;
}

}