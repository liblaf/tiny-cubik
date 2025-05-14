default:

config:
    xmake config --mode="debug"

run:
    xmake
    xmake run --workdir="{{ justfile_directory() }}"
