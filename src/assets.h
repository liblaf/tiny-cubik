#pragma once

#include <memory>

#include "tiny_obj_loader.h"

namespace cubik {

auto load_cube() -> const std::unique_ptr<tinyobj::ObjReader>;

}  // namespace cubik
