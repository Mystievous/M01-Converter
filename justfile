list:
  just --list

clean:
  rm -rf build

build:
  cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
  cmake --build build --config Release