mkdir tmp
cd tmp
git clone https://github.com/y-256/libdivsufsort.git
cd libdivsufsort
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX="/usr/local" ..
make

