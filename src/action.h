#pragma once

#include <cstdint>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

namespace cube {

class Action {
 public:
  // Y Up
  // Z Front
  enum Side { FRONT, BACK, UP, DOWN, LEFT, RIGHT };

  constexpr Action(const Side side, const bool clockwise = true)
      : _side(side), _clockwise(clockwise) {};

  constexpr operator Side() const { return this->_side; }

  auto rotate(const glm::ivec3 location) const -> glm::ivec3;
  auto rotate(const glm::mat4x4 transform, const float progress = 1.0) const
      -> glm::mat4x4;

  constexpr auto axis_index() const -> std::int8_t {
    switch (this->_side) {
      case Action::FRONT:
      case Action::BACK:
        return 2;
      case Action::UP:
      case Action::DOWN:
        return 1;
      case Action::LEFT:
      case Action::RIGHT:
        return 0;
    }
    __builtin_unreachable();
  }

  constexpr auto direction() const -> std::int8_t {
    switch (this->_side) {
      case Action::FRONT:
      case Action::UP:
      case Action::RIGHT:
        return 1;
      case Action::BACK:
      case Action::DOWN:
      case Action::LEFT:
        return -1;
    }
    __builtin_unreachable();
  }

  constexpr auto rotate_angle() const -> float {
    return glm::radians(this->_clockwise ? -90.0 : 90.0);
  }

  constexpr auto rotate_axis() const -> glm::vec3 {
    glm::vec3 axis = glm::zero<glm::vec3>();
    axis[this->axis_index()] = this->direction();
    return axis;
  }

 private:
  bool _clockwise;
  Side _side;
};

}  // namespace cube
