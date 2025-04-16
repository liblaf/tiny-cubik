add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

add_requires("dbg-macro ~0.5.1")
add_requires("eigen ~3.4.0")
add_requires("fmt ~11.1.4")
add_requires("polyscope ~2.3")
add_requires("tinyobjloader v2.0.0rc13")

target("tiny-cubik", function()
  set_kind("binary")
  set_languages("cxx20")
  set_toolchains("clang")
  -- set_warnings("all")

  add_packages("dbg-macro")
  add_packages("eigen")
  add_packages("fmt")
  add_packages("polyscope")
  add_packages("tinyobjloader")

  add_files("src/**.cc")
  add_headerfiles("src/**.h")

  add_configfiles("assets/cube.mtl")
  add_configfiles("assets/cube.obj")
end)
