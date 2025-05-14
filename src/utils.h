#pragma once

#include <array>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <string>

namespace cube {

auto group_name(const glm::ivec3 location) -> std::string;
auto group_name(const int x, const int y, const int z) -> std::string;

constexpr auto locations() -> std::array<glm::ivec3, 26> {
  std::array<glm::ivec3, 26> locations;
  int size = 0;
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      for (int z = -1; z <= 1; ++z) {
        if (x == 0 && y == 0 && z == 0) continue;
        locations[size++] = glm::ivec3(x, y, z);
      }
    }
  }
  return locations;
}

}  // namespace cube
