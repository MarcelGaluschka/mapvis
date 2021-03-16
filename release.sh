cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
make install && ./mapvis 2048 ../audio/ES2002a.Array1-02.wav
cd ..
