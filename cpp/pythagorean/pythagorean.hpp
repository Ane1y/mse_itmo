#pragma once

#include <ranges>

namespace rs = std::ranges;
namespace rv = std::ranges::views;

namespace cls09 {

constexpr inline bool isPythagoryanFlat = false;
    auto Pythagoryan() {
        return rs::iota_view(1) | rv::transform([](const int x) {
            return rs::iota_view(1, x) | rv::transform([x](const int y) {
                return rs::iota_view(1, y + 1) | rv::filter([x, y](const int z){
                    return x * x == y * y + z * z;
                }) | rv::transform([x, y](const int z) {
                    return std::tuple(x, y, z);
                });
            }) | rv::join;
        });
}

} // namespace cls09