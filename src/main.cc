#include <dbg.h>
#include <imgui.h>
#include <polyscope/file_helpers.h>
#include <polyscope/polyscope.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "action.h"
#include "assets.h"
#include "cube.h"
#include "setup.h"

auto callback(std::weak_ptr<cube::Cube> cube) -> void {
  cube.lock()->update();

  ImGui::Text("Action: %s", cube.lock()->action().to_string().c_str());
  ImGui::Text("Animating: %s", cube.lock()->animating() ? "true" : "false");
  ImGui::Text("Recording: %s", cube.lock()->recording() ? "true" : "false");
  ImGui::Text("Replaying: %s", cube.lock()->replaying() ? "true" : "false");
  ImGui::Text("Actions: %s",
              cube::Action::to_string(cube.lock()->records()).c_str());

  if (cube.lock()->idle()) {
    if (ImGui::Button("Replay")) {
      const std::string filepath =
          polyscope::promptForFilename("demo/actions.txt");
      if (!filepath.empty()) {
        std::deque<cube::Action> actions = cube::Action::from_file(filepath);
        cube.lock()->replay(actions);
      }
    } else if (ImGui::Button("Record")) {
      cube.lock()->record_start();
    }
  } else if (cube.lock()->recording()) {
    if (ImGui::Button("Save to File")) {
      const std::string filepath =
          polyscope::promptForFilename("demo/actions.txt");
      if (!filepath.empty()) {
        cube::Action::to_file(filepath, cube.lock()->records());
        spdlog::info("Saved actions to: {}", filepath);
      }
    } else if (ImGui::Button("Stop Recording")) {
      cube.lock()->record_end();
    }
  }

  bool clockwise = !(ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftShift) ||
                     ImGui::IsKeyDown(ImGuiKey::ImGuiKey_RightShift));
  if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_F)) {
    cube.lock()->action(cube::Action(cube::Action::FRONT, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_B)) {
    cube.lock()->action(cube::Action(cube::Action::BACK, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_U)) {
    cube.lock()->action(cube::Action(cube::Action::UP, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_D)) {
    cube.lock()->action(cube::Action(cube::Action::DOWN, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_L)) {
    cube.lock()->action(cube::Action(cube::Action::LEFT, clockwise));
  } else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R)) {
    cube.lock()->action(cube::Action(cube::Action::RIGHT, clockwise));
  }
}

auto main(int argc, char** argv) -> int {
  dbg(ImGui::GetVersion());
  polyscope::init();
  auto obj = cube::load_cube();
  cube::setup(*obj);
  std::shared_ptr<cube::Cube> cube = std::make_shared<cube::Cube>();
  polyscope::state::userCallback = std::bind(callback, cube);
  polyscope::show();
  return 0;
}
