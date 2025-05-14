#include "action.h"

#include <glm/fwd.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

namespace cube {

auto Action::rotate(const glm::ivec3 location) const -> glm::ivec3 {
  glm::vec3 loc = glm::vec3(location);
  loc = glm::rotate(loc, this->rotate_angle(), this->rotate_axis());
  return glm::round(loc);
}

auto Action::rotate(const glm::mat4x4 transform, const float progress) const
    -> glm::mat4x4 {
  return glm::rotate(progress * this->rotate_angle(), this->rotate_axis()) *
         transform;
}

}  // namespace cube
