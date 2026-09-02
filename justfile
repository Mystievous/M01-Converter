list:
  just --list


clean:
  rm -rf build
  rm -rf build-package

build:
  cmake -S . -B build
  cmake --build build -j

package:
    cmake -S . -B build-package -DM01_FLAT_INSTALL=ON -DCMAKE_BUILD_TYPE=Release
    cmake --build build-package -j --config Release
    cmake --build build-package --target package --config Release