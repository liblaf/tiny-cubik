#include "cube.h"

auto make_cube(Eigen::Vector3d center)
    -> std::tuple<Eigen::MatrixX3d, Eigen::MatrixX4i> {
  Eigen::MatrixX3d points(8, 3);
  points << 0, 0, 0, //
      1, 0, 0,       //
      1, 1, 0,       //
      0, 1, 0,       //
      0, 0, 1,       //
      1, 0, 1,       //
      1, 1, 1,       //
      0, 1, 1;
  points.rowwise() += center.transpose();
  Eigen::MatrixX4i triangles(7, 4);
  triangles << 0, 1, 2, 3, //
      4, 5, 6, 7,          //
      0, 1, 5, 4,          //
      1, 2, 6, 5,          //
      2, 3, 7, 6,          //
      3, 0, 4, 7,          //
      0, 1, 2, 3;
  return {points, triangles};
}
