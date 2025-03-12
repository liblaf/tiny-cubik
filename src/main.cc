#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>

#include <Eigen/Dense>

#include "creation/cube.h"

int main(int argc, char** argv) {
  polyscope::init();
  auto [points, triangles] = make_cube();
  polyscope::registerSurfaceMesh("cube", points, triangles);
  polyscope::show();
  return 0;
}
