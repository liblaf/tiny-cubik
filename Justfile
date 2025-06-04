default:

archive:
    git archive --output="archive.zip" HEAD
    zip "archive.zip" "README.pdf"
    zip --recurse-paths "archive.zip" "build/linux/"

config:
    xmake config --mode="debug"

run:
    xmake
    xmake run --workdir="{{ justfile_directory() }}"
