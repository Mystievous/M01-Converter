list:
  just --list


clean:
  rm -rf build
  rm -rf build-package

build:
  cmake -S . -B build
  cmake --build build -j
  cp ./template/config.yml ./build/src/M01-Converter/

[working-directory: 'build/src/M01-Converter/']
run *args='-e': build
  ./M01-Converter {{args}}

package:
    cmake -S . -B build-package -DM01_FLAT_INSTALL=ON -DCMAKE_BUILD_TYPE=Release
    cmake --build build-package -j --config Release
    cmake --build build-package --target package --config Release
