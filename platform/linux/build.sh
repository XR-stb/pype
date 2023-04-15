cd ../../project
python preprocess.py
cd ../platform/linux/build
cmake -Wno-dev ..
make