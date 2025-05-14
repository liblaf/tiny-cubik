#pragma once

#include <tiny_obj_loader.h>

#include <memory>

namespace cube {

auto load_cube() -> const std::unique_ptr<tinyobj::ObjReader>;

}  // namespace cube
