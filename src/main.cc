#include <fmt/format.h>

#include <vector>

#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include "assets.h"
#include "dbg.h"
#include "glm/fwd.hpp"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "tiny_obj_loader.h"

auto bounding_box(const Eigen::MatrixX3f& vertices)
    -> std::tuple<glm::vec3, glm::vec3> {
  Eigen::Vector3f bound_min = vertices.colwise().minCoeff();
  Eigen::Vector3f bound_max = vertices.colwise().maxCoeff();
  glm::vec3 min(bound_min(0), bound_min(1), bound_min(2));
  glm::vec3 max(bound_max(0), bound_max(1), bound_max(2));
  return std::make_tuple(min, max);
}

auto register_cube(const tinyobj::ObjReader& obj) -> void {
  const std::vector<tinyobj::material_t>& materials = obj.GetMaterials();
  const std::vector<tinyobj::shape_t>& shapes = obj.GetShapes();
  const tinyobj::attrib_t& attrib = obj.GetAttrib();
  dbg(attrib.vertices.size());
  Eigen::MatrixX3f vertices(attrib.vertices.size() / 3, 3);
  for (size_t i = 0; i < attrib.vertices.size() / 3; ++i) {
    vertices(i, 0) = attrib.vertices[3 * i + 0];
    vertices(i, 1) = attrib.vertices[3 * i + 1];
    vertices(i, 2) = attrib.vertices[3 * i + 2];
  }
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      for (int k = 0; k < 3; ++k)
        polyscope::createGroup(fmt::format("{}{}{}", i, j, k));
    }
  }
  auto [bound_min, bound_max] = bounding_box(vertices);
  for (const tinyobj::shape_t& shape : shapes) {
    std::vector<std::vector<int>> indices;
    int index_offset = 0;
    glm::vec3 shape_bound_min = glm::vec3(1e10f);
    glm::vec3 shape_bound_max = glm::vec3(-1e10f);
    for (int f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
      const int fv = shape.mesh.num_face_vertices[f];
      std::vector<int> face(fv);
      for (int v = 0; v < fv; v++) {
        const tinyobj::index_t& idx = shape.mesh.indices[index_offset + v];
        face[v] = idx.vertex_index;
        shape_bound_min.x =
            std::min(shape_bound_min.x, vertices(idx.vertex_index, 0));
        shape_bound_min.y =
            std::min(shape_bound_min.y, vertices(idx.vertex_index, 1));
        shape_bound_min.z =
            std::min(shape_bound_min.z, vertices(idx.vertex_index, 2));
        shape_bound_max.x =
            std::max(shape_bound_max.x, vertices(idx.vertex_index, 0));
        shape_bound_max.y =
            std::max(shape_bound_max.y, vertices(idx.vertex_index, 1));
        shape_bound_max.z =
            std::max(shape_bound_max.z, vertices(idx.vertex_index, 2));
      }
      indices.push_back(face);
      index_offset += fv;
    }
    polyscope::SurfaceMesh* mesh =
        polyscope::registerSurfaceMesh(shape.name, vertices, indices);
    // auto [mesh_bound_min, mesh_bound_max] = mesh->boundingBox();
    // Calculate the center of the mesh bounding box
    glm::vec3 mesh_center = (shape_bound_min + shape_bound_max) * 0.5f;

    // Calculate the size of each group
    glm::vec3 group_size = (bound_max - bound_min) / 3.0f;

    // Determine the group indices (i, j, k) for the mesh
    int i = static_cast<int>((mesh_center.x - bound_min.x) / group_size.x);
    int j = static_cast<int>((mesh_center.y - bound_min.y) / group_size.y);
    int k = static_cast<int>((mesh_center.z - bound_min.z) / group_size.z);
    dbg(i, j, k);
    // Clamp indices to ensure they are within bounds [0, 2]
    i = std::clamp(i, 0, 2);
    j = std::clamp(j, 0, 2);
    k = std::clamp(k, 0, 2);

    // Add the mesh to the identified group
    mesh->addToGroup(fmt::format("{}{}{}", i, j, k));
    Eigen::MatrixX3f colors(indices.size(), 3);
    for (size_t i = 0; i < indices.size(); ++i) {
      int material_id = shape.mesh.material_ids[i];
      if (material_id >= 0 && material_id < materials.size()) {
        const tinyobj::material_t& material = materials[material_id];
        colors.row(i) << material.diffuse[0], material.diffuse[1],
            material.diffuse[2];
      } else {
        colors.row(i) << 0.5f, 0.5f,
            0.5f;  // Default color if no material is assigned
      }
    }
    auto color = mesh->addFaceColorQuantity("color", colors);
    color->setEnabled(true);
  }
}

auto main(int argc, char** argv) -> int {
  polyscope::init();
  auto obj = cubik::load_cube();
  register_cube(*obj);
  polyscope::show();
  return 0;
}
