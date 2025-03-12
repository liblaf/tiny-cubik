add_rules("mode.debug", "mode.release")

add_requires("eigen ~3.4.0")
add_requires("polyscope ~2.3")

target("tiny-cubik")
  set_kind("binary")
  set_languages("c++20")
  set_toolchains("clang")

  add_headerfiles("src/**.h")
  add_files("src/**.cc")

  add_packages("eigen")
  add_packages("polyscope")
