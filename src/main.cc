
#include <polyscope/polyscope.h>

#include <memory>

#include "action.h"
#include "assets.h"
#include "cube.h"
#include "imgui.h"
#include "setup.h"

auto callback(std::weak_ptr<cube::Cube> cube) -> void {
  cube.lock()->update();
  bool clockwise = !(ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftShift) ||
                     ImGui::IsKeyDown(ImGuiKey::ImGuiKey_RightShift));
  if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_F)) {
    cube.lock()->start(cube::Action(cube::Action::FRONT, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_B)) {
    cube.lock()->start(cube::Action(cube::Action::BACK, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_U)) {
    cube.lock()->start(cube::Action(cube::Action::UP, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_D)) {
    cube.lock()->start(cube::Action(cube::Action::DOWN, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_L)) {
    cube.lock()->start(cube::Action(cube::Action::LEFT, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R)) {
    cube.lock()->start(cube::Action(cube::Action::RIGHT, clockwise));
  }
}

auto main(int argc, char** argv) -> int {
  polyscope::init();
  auto obj = cube::load_cube();
  cube::setup(*obj);
  std::shared_ptr<cube::Cube> cube = std::make_shared<cube::Cube>();
  polyscope::state::userCallback = std::bind(callback, cube);
  polyscope::show();
  return 0;
}
