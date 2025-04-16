#include "register.h"

#include <utility>

#include "Eigen/Core"
#include "glm/vec3.hpp"

namespace cubik {

template <typename T>
auto bounding_box(const Eigen::MatrixX3<T> &vertices)
    -> std::pair<glm::vec3, glm::vec3> {
  Eigen::Vector3<T> bound_min = vertices.colwise().minCoeff();
  Eigen::Vector3<T> bound_max = vertices.colwise().maxCoeff();
  glm::vec3 min(bound_min(0), bound_min(1), bound_min(2));
  glm::vec3 max(bound_max(0), bound_max(1), bound_max(2));
  return std::make_pair(min, max);
}

auto register_cube(const tinyobj::ObjReader &reader) -> void {
  const tinyobj::attrib_t &attrib = reader.GetAttrib();
  Eigen::MatrixX3f vertices(attrib.vertices.size() / 3, 3);
  for (size_t i = 0; i < attrib.vertices.size() / 3; ++i) {
    vertices(i, 0) = attrib.vertices[3 * i + 0];
    vertices(i, 1) = attrib.vertices[3 * i + 1];
    vertices(i, 2) = attrib.vertices[3 * i + 2];
  }
}

}  // namespace cubik
