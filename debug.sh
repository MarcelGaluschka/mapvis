cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. 
make install && ./mapvis ../audio/ES2002a.Array1-02.wav
cd ..