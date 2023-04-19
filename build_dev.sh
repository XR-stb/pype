cd project
python preprocess.py
cd ..
mkdir -p build/dev
cd build/dev
cmake -Wno-dev ../..
make