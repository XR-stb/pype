python preprocess.py
rm -rf build/web
mkdir -p build/web
cp platform/webassembly/index.html build/web
cd build/web
emcmake cmake ../../CMakeLists.txt
emmake make