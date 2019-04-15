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

#include <memory>
#include <optional>

namespace NRogueLikely {

class TObject {
public:
    TObject() = default;

    TObject(const TObject&) = delete;
    TObject& operator=(const TObject&) = delete;
    TObject(TObject&&) noexcept = default;
    TObject& operator=(TObject&&) noexcept = default;

    std::optional<TPosition> GetPosition() const;
    void SetPosition(const TPosition& position);
    void SetEmptyPosition();

private:
    std::optional<TPosition> Position_;
};

}
