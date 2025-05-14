#include <polyscope/surface_mesh.h>

#include <glm/fwd.hpp>
#include <vector>

#include "action.h"

namespace cube {

class Cubie {
 public:
  explicit Cubie(const glm::ivec3 location);

  auto end(const Action action) -> void;
  auto group() const -> polyscope::Group*;
  auto location() const -> glm::ivec3;
  auto meshes() const -> std::vector<polyscope::SurfaceMesh*>;
  auto name() const -> std::string;
  auto start() -> void;
  auto transform() const -> glm::mat4x4;
  auto update(const Action action, const float progress) -> void;

 private:
  glm::ivec3 _location;
  glm::ivec3 _location_initial;
  glm::mat4x4 _start;
};

}  // namespace cube
