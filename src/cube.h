#pragma once

#include <tiny_obj_loader.h>

#include <chrono>

#include "action.h"
#include "cubie.h"

using namespace std::chrono_literals;

namespace cube {

class Cube {
 public:
  explicit Cube();

  auto action() const -> Action;
  auto animating_cubies() const -> std::vector<std::weak_ptr<Cubie>>;
  auto animating() const -> bool;
  auto end() -> void;
  auto start(const Action action) -> void;
  auto update() -> void;

 private:
  Action _action = Action::FRONT;
  bool _animating = false;
  std::chrono::duration<double> _duration = 0.5s;
  std::chrono::time_point<std::chrono::high_resolution_clock> _animation_start;
  std::vector<std::shared_ptr<Cubie>> _cubies;
};

}  // namespace cube
