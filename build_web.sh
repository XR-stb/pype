rm -rf build/web
mkdir -p build/web
cp platform/webassembly/index.html build/web
cd build/web
cmake -DCMAKE_TOOLCHAIN_FILE=/workspaces/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
       -DCMAKE_BUILD_TYPE=Release \
        ../../CMakeLists.txt

emmake make