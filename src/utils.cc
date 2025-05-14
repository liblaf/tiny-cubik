#include "utils.h"

#include <fmt/format.h>

#include <glm/common.hpp>

namespace cube {

auto group_name(const glm::ivec3 location) -> std::string {
  return group_name(location.x, location.y, location.z);
}

auto group_name(const int x, const int y, const int z) -> std::string {
  return fmt::format("{:2d}{:2d}{:2d}", x, y, z);
}

}  // namespace cube
