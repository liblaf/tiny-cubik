#include <fmt/format.h>
#include <imgui.h>
#include <polyscope/group.h>
#include <polyscope/polyscope.h>
#include <polyscope/structure.h>
#include <polyscope/surface_mesh.h>
#include <polyscope/weak_handle.h>
#include <tiny_obj_loader.h>

#include <Eigen/Dense>
#include <cctype>
#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>

#include "assets.h"
#include "setup.h"

auto select_meshes(const char notation)
    -> std::vector<polyscope::SurfaceMesh*> {
  std::vector<std::string> groups;
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      for (int z = 0; z < 3; z++) {
        switch (std::tolower(notation)) {
          case 'f':
            if (z == 2) groups.push_back(cubik::group_name(x, y, z));
            break;
          case 'b':
            if (z == 0) groups.push_back(cubik::group_name(x, y, z));
            break;
          case 'u':
            if (y == 2) groups.push_back(cubik::group_name(x, y, z));
            break;
          case 'd':
            if (y == 0) groups.push_back(cubik::group_name(x, y, z));
            break;
          case 'l':
            if (x == 0) groups.push_back(cubik::group_name(x, y, z));
            break;
          case 'r':
            if (x == 2) groups.push_back(cubik::group_name(x, y, z));
            break;
        }
      }
    }
  }
  std::vector<polyscope::SurfaceMesh*> meshes;
  for (const std::string& group_name : groups) {
    polyscope::Group* group = polyscope::getGroup(group_name);
    for (const polyscope::WeakHandle<polyscope::Structure> child :
         group->childrenStructures) {
      meshes.push_back(polyscope::getSurfaceMesh(child.get().name));
    }
  }
  return meshes;
}

auto callback() -> void {
  static bool play = true;
  static float rotation = 0.0f;
  ImGui::Checkbox("Play", &play);
  if (!play) return;
  // TODO: read input from keyboard
  static char notation = 'f';
  std::vector<polyscope::SurfaceMesh*> meshes = select_meshes(notation);
  rotation += 0.01f;  // Increment rotation angle
  glm::mat4 transform = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
  for (auto* mesh : meshes) {
    mesh->setTransform(transform);
  }
}

auto main(int argc, char** argv) -> int {
  polyscope::init();
  auto obj = cubik::load_cube();
  cubik::setup_scene(*obj);
  polyscope::state::userCallback = callback;
  polyscope::show();
  return 0;
}
