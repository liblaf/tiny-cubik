#include "setup.h"

#include <Eigen/src/Core/Matrix.h>
#include <fmt/format.h>
#include <polyscope/options.h>
#include <polyscope/polyscope.h>
#include <polyscope/surface_color_quantity.h>
#include <polyscope/surface_mesh.h>
#include <polyscope/types.h>

#include <Eigen/Dense>
#include <glm/fwd.hpp>
#include <string>
#include <utility>
#include <vector>

#include "utils.h"

namespace cube {

auto setup(const tinyobj::ObjReader& reader) -> void {
  register_cube(reader);
  create_groups(reader);
  setup_ground(reader);
}

auto create_groups(const tinyobj::ObjReader& reader) -> void {
  const tinyobj::attrib_t& attrib = reader.GetAttrib();
  const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
  const Eigen::MatrixX3f vertices = load_vertices(attrib.vertices);
  const auto [scene_bound_min, scene_bound_max] = bounding_box(vertices);
  const glm::vec3 cube_center = (scene_bound_max + scene_bound_min) / 2.0f;
  const glm::vec3 cube_length = (scene_bound_max - scene_bound_min) / 3.0f;
  for (glm::ivec3 location : locations()) {
    const std::string name = group_name(location);
    polyscope::createGroup(name);
  }
  for (const tinyobj::shape_t& shape : shapes) {
    const std::vector<std::vector<int>> faces = load_faces(shape.mesh);
    const auto [shape_bound_min, shape_bound_max] =
        bounding_box(vertices, faces);
    const glm::vec3 center = (shape_bound_max + shape_bound_min) / 2.0f;
    const glm::ivec3 location =
        glm::round((center - cube_center) / cube_length);
    const std::string name = group_name(location);
    polyscope::getSurfaceMesh(shape.name)->addToGroup(name);
  }
}

auto register_cube(const tinyobj::ObjReader& reader) -> void {
  const tinyobj::attrib_t& attrib = reader.GetAttrib();
  const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();
  const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
  const Eigen::MatrixX3f vertices = load_vertices(attrib.vertices);
  for (const tinyobj::shape_t& shape : shapes) {
    std::vector<std::vector<int>> indices = load_faces(shape.mesh);
    polyscope::SurfaceMesh* mesh =
        polyscope::registerSurfaceMesh(shape.name, vertices, indices);
    Eigen::MatrixX3f colors = load_diffuse(materials, shape.mesh);
    polyscope::SurfaceFaceColorQuantity* diffuse =
        mesh->addFaceColorQuantity("diffuse", colors);
    diffuse->setEnabled(true);
  }
}

auto setup_ground(const tinyobj::ObjReader& reader) -> void {
  const tinyobj::attrib_t& attrib = reader.GetAttrib();
  const Eigen::MatrixX3f vertices = load_vertices(attrib.vertices);
  const auto [scene_bound_min, scene_bound_max] = bounding_box(vertices);
  const glm::vec3 size = scene_bound_max - scene_bound_min;
  polyscope::options::groundPlaneHeight = scene_bound_min.z - (size.z / 2.0);
  polyscope::options::groundPlaneHeightMode =
      polyscope::GroundPlaneHeightMode::Manual;
}

auto bounding_box(const Eigen::MatrixX3f& vertices)
    -> std::pair<glm::vec3, glm::vec3> {
  Eigen::Vector3f bound_min = vertices.colwise().minCoeff();
  Eigen::Vector3f bound_max = vertices.colwise().maxCoeff();
  const glm::vec3 min(bound_min(0), bound_min(1), bound_min(2));
  const glm::vec3 max(bound_max(0), bound_max(1), bound_max(2));
  return std::make_pair(min, max);
}

auto bounding_box(const Eigen::MatrixX3f& vertices,
                  const std::vector<std::vector<int>>& faces)
    -> std::pair<glm::vec3, glm::vec3> {
  Eigen::Vector3f bound_min =
      Eigen::Vector3f::Constant(std::numeric_limits<float>::max());
  Eigen::Vector3f bound_max =
      Eigen::Vector3f::Constant(std::numeric_limits<float>::lowest());
  for (const std::vector<int>& face : faces) {
    for (const int vid : face) {
      const Eigen::Vector3f& vertex = vertices.row(vid);
      bound_min = bound_min.cwiseMin(vertex);
      bound_max = bound_max.cwiseMax(vertex);
    }
  }
  const glm::vec3 min(bound_min(0), bound_min(1), bound_min(2));
  const glm::vec3 max(bound_max(0), bound_max(1), bound_max(2));
  return std::make_pair(min, max);
}

auto load_vertices(const std::vector<float>& vertices) -> Eigen::MatrixX3f {
  Eigen::MatrixX3f result(vertices.size() / 3, 3);
  for (size_t i = 0; i < vertices.size() / 3; ++i) {
    result(i, 0) = vertices[3 * i + 0];
    result(i, 1) = vertices[3 * i + 1];
    result(i, 2) = vertices[3 * i + 2];
  }
  return result;
}

auto load_faces(const tinyobj::mesh_t& mesh) -> std::vector<std::vector<int>> {
  std::vector<std::vector<int>> indices;
  int index_offset = 0;
  for (int f = 0; f < mesh.num_face_vertices.size(); f++) {
    const int fv = mesh.num_face_vertices[f];
    std::vector<int> face(fv);
    for (int v = 0; v < fv; v++) {
      const tinyobj::index_t& idx = mesh.indices[index_offset + v];
      face[v] = idx.vertex_index;
    }
    indices.push_back(face);
    index_offset += fv;
  }
  return indices;
}

auto load_diffuse(const std::vector<tinyobj::material_t>& materials,
                  const tinyobj::mesh_t& mesh) -> Eigen::MatrixX3f {
  const int n_faces = mesh.num_face_vertices.size();
  Eigen::MatrixX3f colors(n_faces, 3);
  for (size_t i = 0; i < n_faces; ++i) {
    int material_id = mesh.material_ids[i];
    const tinyobj::material_t material = materials[material_id];
    colors.row(i) << material.diffuse[0], material.diffuse[1],
        material.diffuse[2];
  }
  return colors;
}

}  // namespace cube
