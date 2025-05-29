#pragma once

#include <tiny_obj_loader.h>

#include <chrono>
#include <deque>

#include "action.h"
#include "cubie.h"

namespace cube {

class Cube {
 public:
  enum State { IDLE, RECORDING, REPLAYING };

  explicit Cube();

  auto action() const -> Action;
  auto action(const Action action) -> void;
  auto animate_end() -> void;
  auto animating_cubies() const -> std::vector<std::weak_ptr<Cubie>>;
  auto animating() const -> bool;
  auto idle() const -> bool;
  auto record_end() -> void;
  auto record_start() -> void;
  auto recording() const -> bool;
  auto records() const -> std::deque<Action>;
  auto replay(const std::deque<Action>& actions) -> void;
  auto replaying() const -> bool;
  auto state() const -> State;
  auto update() -> void;

 private:
  Action _action = Action::FRONT;
  bool _animating = false;
  State _state = IDLE;
  std::chrono::duration<double> _duration = std::chrono::duration<double>(0.5);
  std::chrono::time_point<std::chrono::high_resolution_clock> _animation_start;
  std::vector<std::shared_ptr<Cubie>> _cubies;

  std::deque<Action> _actions;
};

}  // namespace cube
