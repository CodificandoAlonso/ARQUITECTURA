cmake -S ./ -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake -S ./ -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-debug
cmake --build cmake-build-release
make -C cmake-build-debug
make -C cmake-build-release