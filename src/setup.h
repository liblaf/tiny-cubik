#pragma once

#include <tiny_obj_loader.h>

#include <Eigen/Core>
#include <glm/fwd.hpp>
#include <string>

namespace cubik {

auto bounding_box(const Eigen::MatrixX3f& vertices)
    -> std::pair<glm::vec3, glm::vec3>;
auto bounding_box(const Eigen::MatrixX3f& vertices,
                  const std::vector<std::vector<int>>& faces)
    -> std::pair<glm::vec3, glm::vec3>;

auto load_vertices(std::vector<float> vertices) -> Eigen::MatrixX3f;
auto load_faces(const tinyobj::mesh_t& mesh) -> std::vector<std::vector<int>>;
auto load_diffuse(const std::vector<tinyobj::material_t>& materials,
                  const tinyobj::mesh_t& mesh) -> Eigen::MatrixX3f;

auto group_name(const glm::ivec3 grid) -> std::string;
auto group_name(const int x, const int y, const int z) -> std::string;

auto setup_ground(const tinyobj::ObjReader& reader) -> void;
auto register_cube(const tinyobj::ObjReader& reader) -> void;
auto create_groups(const tinyobj::ObjReader& reader) -> void;
auto setup_scene(const tinyobj::ObjReader& reader) -> void;

}  // namespace cubik
