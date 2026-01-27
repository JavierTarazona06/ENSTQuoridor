#pragma once

#include <array>
#include <string_view>
#include <cstdint>

namespace Quoridor {

// Identifier for players. Aligns with existing indices (0 = top/start, 1 = bottom).
enum class PlayerId : std::uint8_t {
    Red = 0,
    Blue = 1
};

// Shared mapping of player display names.
constexpr std::array<std::string_view, 2> PLAYER_NAMES = {"Red", "Blue"};

} // namespace Quoridor
