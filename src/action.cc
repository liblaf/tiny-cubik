#include "action.h"

#include <spdlog/spdlog.h>

#include <cctype>
#include <deque>
#include <fstream>
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

auto Action::from_file(const std::string &filepath) -> std::deque<Action> {
  std::deque<Action> actions;
  std::ifstream file(filepath);
  if (!file.is_open()) {
    spdlog::error("Failed to open: {}", filepath);
    return actions;
  }
  for (char ch = file.get(); file; ch = file.get()) {
    ch = std::toupper(ch);
    switch (ch) {
      case 'F': {
        actions.emplace_back(Action::FRONT);
        break;
      }
      case 'B': {
        actions.emplace_back(Action::BACK);
        break;
      }
      case 'U': {
        actions.emplace_back(Action::UP);
        break;
      }
      case 'D': {
        actions.emplace_back(Action::DOWN);
        break;
      }
      case 'L': {
        actions.emplace_back(Action::LEFT);
        break;
      }
      case 'R': {
        actions.emplace_back(Action::RIGHT);
        break;
      }
      case '\'': {
        if (!actions.empty()) {
          actions.back().clockwise = false;
        }
        break;
      }
      case '2': {
        if (!actions.empty()) {
          actions.push_back(actions.back());
          actions.back().clockwise = true;
        }
        break;
      }
      default:
        if (!std::isspace(ch)) {
          spdlog::warn("Ignoring unrecognized character: `{}`.", ch);
        }
    }
  }
  file.close();
  return actions;
}

auto Action::to_file(const std::string &filepath,
                     const std::deque<Action> &actions) -> void {
  std::ofstream file(filepath);
  if (!file.is_open()) {
    spdlog::error("Failed to open: {}", filepath);
    return;
  }
  for (const Action &action : actions) {
    file << action.to_string() << ' ';
  }
  file << std::endl;
  file.close();
}

auto Action::to_string(const std::deque<Action> &actions) -> std::string {
  std::string str;
  for (const Action &action : actions) {
    str += action.to_string() + ' ';
  }
  return str;
}

auto Action::to_string() const -> std::string {
  std::string str;
  switch (this->side) {
    case Action::FRONT:
      str += 'F';
      break;
    case Action::BACK:
      str += 'B';
      break;
    case Action::UP:
      str += 'U';
      break;
    case Action::DOWN:
      str += 'D';
      break;
    case Action::LEFT:
      str += 'L';
      break;
    case Action::RIGHT:
      str += 'R';
      break;
  }
  if (!this->clockwise) {
    str += '\'';
  }
  return str;
}

}  // namespace cube
