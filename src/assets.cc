#include "assets.h"

#include <spdlog/spdlog.h>

#include <memory>

namespace cube {

const std::unique_ptr<tinyobj::ObjReader> load_cube() {
  std::unique_ptr<tinyobj::ObjReader> reader =
      std::make_unique<tinyobj::ObjReader>();
  tinyobj::ObjReaderConfig config;
  config.triangulate = false;
  bool ok = reader->ParseFromFile("assets/cube.obj", config);
  if (!ok) {
    if (!reader->Error().empty()) spdlog::error(reader->Error());
    std::terminate();
  }
  if (!reader->Warning().empty()) spdlog::warn(reader->Warning());
  return reader;
}

}  // namespace cube
