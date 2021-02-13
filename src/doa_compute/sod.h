#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>
#include <complex>
#include <math.h>

class SOD
{
    public: 
        SOD(int samplerate, uint16_t BUFFER_SIZE, int angles_x);
        void compute(std::array<int16_t*,8> *buffers);


    private:
        double hamming(double windowsize, int pos);


        std::array<std::complex<double>,8> mics;

        double samplerate;
        int k;

        int angles_x;

        const double windowsize = 512;
        std::array<double, 512> hamming_array;
        const double airspeed = 343;

};

