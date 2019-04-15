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

#include <ui/ncurses/ncurses_ui.h>

#include <object/object.h>
#include <world/world_view.h>

#include <ncurses.h>

namespace NRogueLikely {

class TNCursesUI::TImpl {
private:
    void DrawCellAt(int y, int x, TCellPtr cell) {
        switch(cell->GetFloorType()) {
            case ECellFloor::EMPTY:
                mvaddch(y, x, ' ');
            case ECellFloor::FLOOR:
                mvaddch(y, x, '.');
            case ECellFloor::WALL:
                mvaddch(y, x, '#');
            case ECellFloor::PASSAGE:
                mvaddch(y, x, '.');
        }
    }

public:
    TImpl() {
        // Initialize the ncurses library
        initscr();
        // Disable line buffering. Note: this doesn't disable control characters (Ctrl+C, Ctrl+Z). In order to disable
        // them, raw() should be used.
        cbreak();
        // Disable echoing of user characters.
        echo();
        // Enable arrows for the main screen.
        keypad(stdscr, TRUE);
    }

    ~TImpl() {
        // Deinitialize the ncurses library
        endwin();
    }

    void DrawMap(const TWorld& world, TObjectPtr objectPtr) {
        TWorldView view = world.GetViewOf(objectPtr);
        for (int i = 0; i < view.GetHeight(); i++) {
            for (int j = 0; j < view.GetWidth(); j++) {
                DrawCellAt(i, j, view.GetCell(i, j));
            }
        }
        refresh();
    }
};

TNCursesUI::TNCursesUI()
    : impl_(std::make_unique<TImpl>())
{ }

TNCursesUI::~TNCursesUI() = default;

void TNCursesUI::DrawMap(const TWorld& world, TObjectPtr objectPtr) {
    impl_->DrawMap(world, objectPtr);
}

TUserActionPtr TNCursesUI::AwaitUserAction() {
    getch();
    return nullptr;
}

}
