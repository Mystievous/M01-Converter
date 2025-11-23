list:
  just --list

clean:
  rm -rf build

build:
  cmake -S . -B build
  cmake --build build -j