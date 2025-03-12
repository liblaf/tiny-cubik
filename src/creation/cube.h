#pragma once

#include <tuple>

#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"

auto make_cube(Eigen::Vector3d center = Eigen::Vector3d::Zero())
    -> std::tuple<Eigen::MatrixX3d, Eigen::MatrixX4i>;
