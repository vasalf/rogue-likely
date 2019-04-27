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

#include <world/world_generator.h>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <map>
#include <queue>
#include <random>
#include <set>

namespace NRogueLikely {

TPlainWorldGenerator::TPlainWorldGenerator(int levels, int height, int width)
    : Levels_(levels)
    , Height_(height)
    , Width_(width)
{}

TWorld TPlainWorldGenerator::Generate() {
    TWorld ret(Levels_, Height_, Width_);
    for (int i = 0; i < Levels_; i++) {
        for (int k = 0; k < Width_; k++) {
            ret.SetCell(i, 0, k, MakeCell(ECellFloor::WALL));
            ret.SetCell(i, Height_ - 1, k, MakeCell(ECellFloor::WALL));
        }
        for (int j = 1; j < Height_ - 1; j++) {
            ret.SetCell(i, j, 0, MakeCell(ECellFloor::WALL));
            for (int k = 1; k < Width_ - 1; k++) {
                ret.SetCell(i, j, k, MakeCell(ECellFloor::FLOOR));
            }
            ret.SetCell(i, j, Width_ - 1, MakeCell(ECellFloor::WALL));
        }
        if (i > 0) {
            ret.SetCell(i, 1, 2, MakeCell(ECellFloor::LADDER_UP));
        }
        if (i < Levels_ - 1) {
            ret.SetCell(i, 1, 1, MakeCell(ECellFloor::LADDER_DOWN));
        }
    }
    ret.SetStartPosition(TPosition(0, Height_ / 2, Width_ / 2));
    return ret;
}

TLoadWorldGenerator::TLoadWorldGenerator(const std::string& filename)
    : Filename_(filename)
{ }

TWorld TLoadWorldGenerator::Generate() {
    std::ifstream file(Filename_);
    int levels, height, width;
    file >> levels >> height >> width;
    TPosition startPosition;
    file >> startPosition.Level >> startPosition.I >> startPosition.J;
    TWorld world(levels, height, width);
    world.SetStartPosition(startPosition);
    for (int i = 0; i < levels; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                int x;
                file >> x;
                auto floor = (ECellFloor)x;
                world.SetCell(i, j, k, MakeCell(floor));
            }
        }
    }
    return world;
}

namespace {

struct TRoom {
    TPosition UpperLeft;
    TPosition DownRight;
};

void DrawRoom(TWorld& world, const TRoom& room) {
    assert(room.UpperLeft.Level == room.DownRight.Level);
    for (int i = room.UpperLeft.I + 1; i < room.DownRight.I; i++) {
        world.SetCell(room.UpperLeft.Level, i, room.UpperLeft.J, MakeCell(ECellFloor::WALL));
        for (int j = room.UpperLeft.J + 1; j < room.DownRight.J; j++) {
            world.SetCell(room.UpperLeft.Level, i, j, MakeCell(ECellFloor::FLOOR));
        }
        world.SetCell(room.UpperLeft.Level, i, room.DownRight.J, MakeCell(ECellFloor::WALL));
    }
    for (int j = room.UpperLeft.J; j <= room.DownRight.J; j++) {
        world.SetCell(room.UpperLeft.Level, room.UpperLeft.I, j, MakeCell(ECellFloor::WALL));
        world.SetCell(room.UpperLeft.Level, room.DownRight.I, j, MakeCell(ECellFloor::WALL));
    }
}

template<class Rng>
int GenerateRoomBound(int x, int y, Rng& rnd) {
    std::uniform_int_distribution dist(x, y);
    return dist(rnd);
}

template<class Rng>
TRoom GenerateRoomAround(int height, int width, TPosition point, Rng& rng) {
    TRoom ret;
    ret.UpperLeft.Level = point.Level;
    ret.DownRight.Level = point.Level;
    int x = point.I, y = point.J;
    int maxHeight = (height + 3) / 4;
    int maxWidth = (width + 3) / 4;
    ret.UpperLeft.I = GenerateRoomBound(std::max(0, x - 1 - (maxHeight + 1) / 2), x - 1, rng);
    ret.DownRight.I = GenerateRoomBound(x + 1, std::min(height - 1, x + 1 + (maxHeight + 1) / 2), rng);
    ret.UpperLeft.J = GenerateRoomBound(std::max(0, y - 1 - (maxWidth + 1) / 2), y - 1, rng);
    ret.DownRight.J = GenerateRoomBound(y + 1, std::min(width - 1, y + 1 + (maxHeight + 1) / 2), rng);
    return ret;
}

bool RoomIntersectsWithSome(const TWorld& world, const TRoom& room) {
    assert(room.UpperLeft.Level == room.DownRight.Level);
    for (int i = room.UpperLeft.I; i <= room.DownRight.I; i++) {
        for (int j = room.UpperLeft.J; j <= room.DownRight.J; j++) {
            if (world.GetCell(room.UpperLeft.Level, i, j)->GetFloorType() == ECellFloor::EMPTY) {
                continue;
            } else if (world.GetCell(room.UpperLeft.Level, i, j)->GetFloorType() == ECellFloor::WALL) {
                if (i == room.UpperLeft.I || i == room.DownRight.I || j == room.UpperLeft.J || j == room.DownRight.J) {
                    continue;
                }
                return true;
            } else {
                return true;
            }
        }
    }
    return false;
}

template<class Rng>
TRoom GenerateRoom(const TWorld& world, int level, Rng& rnd) {
    std::uniform_int_distribution<int> heightDist(1, world.GetLevelHeight() - 2);
    std::uniform_int_distribution<int> widthDist(1, world.GetLevelWidth() - 2);
    while (true) {
        TPosition position(level, heightDist(rnd), widthDist(rnd));
        TRoom room = GenerateRoomAround(world.GetLevelHeight(), world.GetLevelWidth(), position, rnd);
        if (!RoomIntersectsWithSome(world, room)) {
            return room;
        }
    }
}

int NumNonEmpty(const TWorld& world, int level) {
    int ret = 0;
    for (int i = 0; i < world.GetLevelHeight(); i++) {
        for (int j = 0; j < world.GetLevelWidth(); j++) {
            if (world.GetCell(level, i, j)->GetFloorType() != ECellFloor::EMPTY) {
                ret++;
            }
        }
    }
    return ret;
}

template<class Rng>
TPosition RandomFloorCell(const TWorld& world, int level, Rng& rnd) {
    std::vector<TPosition> candidates;
    for (int i = 0; i < world.GetLevelHeight(); i++) {
        for (int j = 0; j < world.GetLevelWidth(); j++) {
            if (world.GetCell(level, i, j)->GetFloorType() == ECellFloor::FLOOR)
                candidates.push_back({level, i, j});
        }
    }
    std::uniform_int_distribution<int> dist(0, (int)candidates.size() - 1);
    return candidates[dist(rnd)];
}

template<class Rng>
TPosition RandomAtBound(const TRoom& room, Rng& rnd) {
    int h = room.DownRight.I - room.UpperLeft.I;
    int w = room.DownRight.J - room.UpperLeft.J;
    std::uniform_int_distribution dist(0, 2 * (h + w) - 5);
    int i = dist(rnd);
    if (i < h - 1)
        return TPosition(room.UpperLeft.Level, room.UpperLeft.I + 1 + i, room.UpperLeft.J);
    else if (i < h + w - 2)
        return TPosition(room.UpperLeft.Level, room.DownRight.I, room.UpperLeft.J + 1 + i - (h - 1));
    else if (i < h + w + h - 3)
        return TPosition(room.UpperLeft.Level, room.UpperLeft.I + 1 + i - (h + w - 2), room.DownRight.J);
    else
        return TPosition(room.UpperLeft.Level, room.UpperLeft.I, room.UpperLeft.J + 1 + i - (h + w + h - 3));
}

bool IsInRoom(const TRoom& room, TPosition position) {
    return position.Level == room.UpperLeft.Level
            && position.I >= room.UpperLeft.I && position.I <= room.DownRight.I
            && position.J >= room.UpperLeft.J && position.J <= room.DownRight.J;
}

template<class Rng>
bool RandomPath(TWorld& world, TPosition cur, const TRoom& restricted, Rng& rnd) {
    std::uniform_int_distribution<int> edgeDist(0, 1000);
    std::map<TPosition, int> dist;
    std::map<TPosition, TPosition> prev;
    std::set<TPosition> visited;
    std::priority_queue<std::pair<int, TPosition>> queue;
    dist[cur] = 0;
    queue.push(std::make_pair(0, cur));
    TPosition best(-1, -1, -1);
    while (!queue.empty()) {
        auto p = queue.top();
        queue.pop();
        if (visited.count(p.second)) {
            continue;
        }
        visited.insert(p.second);
        if (p.second != cur && (world.GetCell(p.second)->GetFloorType() == ECellFloor::FLOOR
                               || world.GetCell(p.second)->GetFloorType() == ECellFloor::PASSAGE)) {
            best = p.second;
            break;
        }
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; i++) {
            TPosition next(p.second.Level, p.second.I + dx[i], p.second.J + dy[i]);
            if (next.I < 0 || next.I >= world.GetLevelHeight() || next.J < 0 || next.J >= world.GetLevelWidth()) {
                continue;
            }
            if (IsInRoom(restricted, next)) {
                continue;
            }
            int e = edgeDist(rnd);
            if (!dist.count(next) || dist[next] > dist[p.second] + e) {
                dist[next] = dist[p.second] + e;
                prev[next] = p.second;
                queue.push(std::make_pair(-dist[next], next));
            }
        }
    }
    if (best.Level > -1) {
        world.SetCell(best, MakeCell(ECellFloor::PASSAGE));
        while (prev.count(best)) {
            best = prev[best];
            world.SetCell(best, MakeCell(ECellFloor::PASSAGE));
        }
        return true;
    } else {
        return false;
    }
}

}

TRoomedWorldGenerator::TRoomedWorldGenerator(int levels, int height, int width)
    : Levels_(levels)
    , Height_(height)
    , Width_(width)
{ }

TWorld TRoomedWorldGenerator::Generate() {
    std::random_device rd;
    std::mt19937 rnd(rd());
    TWorld world(Levels_, Height_, Width_);

    TPosition start(0, Height_ / 2, Width_ / 2);
    world.SetStartPosition(start);
    for (int i = 0; i < Levels_; i++) {
        TRoom startRoom = GenerateRoomAround(Height_,  Width_, start, rnd);
        DrawRoom(world, startRoom);
        if (i != 0) {
            world.SetCell(start, MakeCell(ECellFloor::LADDER_UP));
        }
        while (NumNonEmpty(world, i) < 0.4 * Height_ * Width_) {
            TRoom room = GenerateRoom(world, i, rnd);
            DrawRoom(world, room);
            while (true) {
                TPosition pathStart = RandomAtBound(room, rnd);
                if (RandomPath(world, pathStart, room, rnd)) {
                    break;
                }
            }
        }
        if (i != Levels_ - 1) {
            start = RandomFloorCell(world, i, rnd);
            world.SetCell(start, MakeCell(ECellFloor::LADDER_DOWN));
            start.Level++;
        }
    }

    return world;
}

}
