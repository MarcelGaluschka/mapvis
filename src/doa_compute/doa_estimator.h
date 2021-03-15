#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>
#include <complex>
#include <math.h>
#include <algorithm>

class DOA_EST
{
    public:
        DOA_EST (){};

        virtual std::array<double,3> compute(std::array<int16_t*,8> *buffers) = 0;

    protected:

        double hamming(double windowsize, int pos);

        void fft(std::vector<std::complex<double>> &x);
        void ifft(std::vector<std::complex<double>> &x);

};

