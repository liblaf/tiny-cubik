#include "cubie.h"

#include <dbg.h>
#include <polyscope/group.h>
#include <polyscope/structure.h>
#include <polyscope/surface_mesh.h>
#include <polyscope/weak_handle.h>

#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>

#include "action.h"
#include "utils.h"

namespace cube {

Cubie::Cubie(const glm::ivec3 location)
    : _location(location), _location_initial(location) {}

auto Cubie::end(const Action action) -> void {
  if (this->_location_initial == glm::ivec3(1, 1, 1)) {
    dbg(this->name(), glm::to_string(this->transform()));
  }
  this->_location = action.rotate(this->_location);
}

auto Cubie::group() const -> polyscope::Group* {
  return polyscope::getGroup(this->name());
}

auto Cubie::location() const -> glm::ivec3 { return this->_location; }

auto Cubie::meshes() const -> std::vector<polyscope::SurfaceMesh*> {
  std::vector<polyscope ::SurfaceMesh*> meshes;
  for (const polyscope::WeakHandle<polyscope::Structure> handle :
       this->group()->childrenStructures)
    meshes.push_back(polyscope::getSurfaceMesh(handle.get().name));
  return meshes;
}

auto Cubie::name() const -> std::string {
  return group_name(this->_location_initial);
}

auto Cubie::start() -> void { this->_start = this->transform(); }

auto Cubie::transform() const -> glm::mat4x4 {
  return polyscope::getSurfaceMesh(
             this->group()->childrenStructures.front().get().name)
      ->getTransform();
}

auto Cubie::update(const Action action, const float progress) -> void {
  for (polyscope::SurfaceMesh* mesh : this->meshes()) {
    glm::mat4x4 transform = action.rotate(this->_start, progress);
    mesh->setTransform(transform);
  }
}

}  // namespace cube
