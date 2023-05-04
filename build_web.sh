python preprocess.py
rm -rf build/web
mkdir -p build/web
cp platform/webassembly/index.html build/web
cp -r game build/web
cd build/web
emcmake cmake ../../CMakeLists.txt
emmake make
rm -rf CMakeFiles
rm -rf CMakeCache.txt
rm -rf Makefile
rm -rf cmake_install.cmake
rm -rf game