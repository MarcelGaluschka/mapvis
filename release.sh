mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
make install
cd ..
cp build/mapvis .
