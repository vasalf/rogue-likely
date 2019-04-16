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

#include <object/alive_object.h>
#include <object/object.h>
#include <world/world_view.h>

#include <ncurses.h>

namespace NRogueLikely {
namespace {

class TMoveAction final : public IUserAction {
public:
    enum class EMoveDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        LEVEL_UP,
        LEVEL_DOWN
    };

    TMoveAction(EMoveDirection direction)
        : Direction_(direction)
    { }

    bool Perform(TWorld& world, TAliveObjectPtr object) override {
        TPosition newPosition = object->GetPositionSafe();
        switch(Direction_) {
            case EMoveDirection::LEFT:
                newPosition.J--;
                break;
            case EMoveDirection::RIGHT:
                newPosition.J++;
                break;
            case EMoveDirection::UP:
                newPosition.I--;
                break;
            case EMoveDirection::DOWN:
                newPosition.I++;
                break;
            case EMoveDirection::LEVEL_UP:
                newPosition.Level--;
                break;
            case EMoveDirection::LEVEL_DOWN:
                newPosition.Level++;
                break;
        }
        return world.TryToMoveObject(object, newPosition);
    }

private:
    EMoveDirection Direction_;
};

}

class TNCursesUI::TImpl {
private:
    char GetCellFloorChar(const TCellPtr& cell) {
        switch(cell->GetFloorType()) {
            case ECellFloor::EMPTY:
                return ' ';
            case ECellFloor::FLOOR:
                return '.';
            case ECellFloor::WALL:
                return '#';
            case ECellFloor::PASSAGE:
                return '.';
            case ECellFloor::LADDER_UP:
                return '>';
            case ECellFloor::LADDER_DOWN:
                return '<';
        }
    }

    std::optional<char> GetCellObjectChar(const TCellPtr& cell) {
        for (const auto& objPtr : cell->GetObjectsHere()) {
            if (objPtr->GetObjectType() == EObjectType::PLAYER) {
                return '@';
            }
        }
        return {};
    }

    void DrawCellAt(int y, int x, const TCellPtr& cell) {
        auto objectChar = GetCellObjectChar(cell);
        if (objectChar.has_value()) {
            mvaddch(y, x, objectChar.value());
        } else {
            mvaddch(y, x, GetCellFloorChar(cell));
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
        noecho();
        // Enable arrows for the main screen.
        keypad(stdscr, TRUE);
    }

    ~TImpl() {
        // Deinitialize the ncurses library
        endwin();
    }

    void DrawMap(const TWorld& world, const TObjectPtr& objectPtr) {
        TWorldView view = world.GetViewOf(objectPtr);
        for (int i = 0; i < view.GetHeight(); i++) {
            for (int j = 0; j < view.GetWidth(); j++) {
                DrawCellAt(i, j, view.GetCell(i, j));
            }
        }
        refresh();
    }

    TUserActionPtr AwaitUserAction() {
        while (true) {
            int ch = getch();
            switch(ch) {
                case KEY_LEFT:
                    return std::make_shared<TMoveAction>(TMoveAction::EMoveDirection::LEFT);
                case KEY_RIGHT:
                    return std::make_shared<TMoveAction>(TMoveAction::EMoveDirection::RIGHT);
                case KEY_UP:
                    return std::make_shared<TMoveAction>(TMoveAction::EMoveDirection::UP);
                case KEY_DOWN:
                    return std::make_shared<TMoveAction>(TMoveAction::EMoveDirection::DOWN);
                case '>':
                    return std::make_shared<TMoveAction>(TMoveAction::EMoveDirection::LEVEL_DOWN);
                case '<':
                    return std::make_shared<TMoveAction>(TMoveAction::EMoveDirection::LEVEL_UP);
            }
        }
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
    return impl_->AwaitUserAction();
}

}
