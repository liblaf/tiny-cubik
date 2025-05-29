#include "cube.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <memory>

namespace cube {

Cube::Cube() {
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      for (int z = -1; z <= 1; ++z) {
        this->_cubies.push_back(std::make_shared<Cubie>(glm::ivec3(x, y, z)));
      }
    }
  }
}

auto Cube::action() const -> Action { return this->_action; }

auto Cube::action(const Action action) -> void {
  if (this->animating()) {
    spdlog::debug("Cube is already animating.");
    return;
  }
  if (this->recording()) this->_actions.push_back(action);
  this->_action = action;
  this->_animating = true;
  this->_animation_start = std::chrono::high_resolution_clock::now();
  for (const std::weak_ptr<Cubie> cubie : this->animating_cubies())
    cubie.lock()->start();
}

auto Cube::animate_end() -> void {
  for (const std::weak_ptr<Cubie> cubie : this->animating_cubies())
    cubie.lock()->end(this->action());
  this->_animating = false;
}

auto Cube::animating_cubies() const -> std::vector<std::weak_ptr<Cubie>> {
  std::vector<std::weak_ptr<Cubie>> animating_cubies;
  const Action action = this->action();
  const int axis = action.axis_index();
  const int direction = action.direction();
  for (const std::shared_ptr<Cubie> cubie : this->_cubies) {
    if (cubie->location()[axis] == direction) animating_cubies.push_back(cubie);
  }
  return animating_cubies;
}

auto Cube::animating() const -> bool { return this->_animating; }

auto Cube::idle() const -> bool { return this->_state == State::IDLE; }

auto Cube::record_end() -> void {
  this->_actions.clear();
  this->_state = State::IDLE;
}

auto Cube::record_start() -> void {
  if (this->_state != State::IDLE) {
    spdlog::warn("Cube is already recording or replaying.");
    return;
  }
  this->_state = State::RECORDING;
  this->_actions.clear();
  spdlog::info("Recording started.");
}

auto Cube::recording() const -> bool {
  return this->_state == State::RECORDING;
}

auto Cube::records() const -> std::deque<Action> { return this->_actions; }

auto Cube::replay(const std::deque<Action>& actions) -> void {
  if (this->_state != State::IDLE) {
    spdlog::warn("Cube is already recording or replaying.");
    return;
  }
  if (actions.empty()) {
    spdlog::warn("No actions to replay.");
    return;
  }
  this->_actions = actions;
  this->_state = State::REPLAYING;
  this->action(this->_actions.front());
  this->_actions.pop_front();
}

auto Cube::replaying() const -> bool {
  return this->_state == State::REPLAYING;
}

auto Cube::update() -> void {
  if (!this->animating()) return;
  std::chrono::time_point now = std::chrono::high_resolution_clock::now();
  std::chrono::duration elapsed = now - this->_animation_start;
  float progress = elapsed / this->_duration;
  bool end = progress >= 1.0;
  if (end) progress = 1.0;
  for (std::weak_ptr<Cubie> cubie : this->animating_cubies())
    cubie.lock()->update(this->action(), progress);
  if (end) {
    this->animate_end();
    if (this->replaying()) {
      if (!this->_actions.empty()) {
        this->action(this->_actions.front());
        this->_actions.pop_front();
      } else {
        this->_state = State::IDLE;
        spdlog::info("Replay finished.");
      }
    }
  }
}

}  // namespace cube
