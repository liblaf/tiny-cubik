#pragma once

#include "tiny_obj_loader.h"

namespace cubik {

auto register_cube(const tinyobj::ObjReader& reader) -> void;

}  // namespace cubik
