#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>
#include <complex>
#include <math.h>
#include <algorithm>

/*
* Sum of delay algorithms in tbe 2D - case
*/ 
class SOD
{
    public: 
        SOD(int samplerate, uint16_t BUFFER_SIZE, int angles_x);
        std::array<double,3> compute(std::array<int16_t*,8> *buffers);

    private:
        double hamming(double windowsize, int pos);

        void fft(std::vector<std::complex<double>> &x);
        void ifft(std::vector<std::complex<double>> &x);

        std::vector<std::complex<double>> mics;
        int num_mics = 8;
        double samplerate;
        int k;

        int angles_x;
        std::vector<double> act_angles_x;

        const double windowsize = 512;
        std::vector<double> hamming_array;
        const double airspeed = 343;
        uint16_t BUFFER_SIZE;

        std::vector<std::vector<double>> xa;
        std::vector<std::vector<std::complex<double>>> ya;
        std::vector<std::vector<double>> y;

        std::vector<double> temp_xa;
        std::vector<std::complex<double>> temp_xa_comp;
        std::vector<double> temp_y;

        std::vector<double> l_array;

        std::array<double,2> incoming_ray;

        std::vector<double> inc_matrix;
        std::vector<double> inc_matrix2;

        std::vector<std::vector<std::complex<double>>> H;
        std::vector<double> Th;


        std::vector<double> temp_steering_block;
        std::vector<std::vector<double>> steering_block;

        double current_sum;
        double current_max_angl; 
        double current_max_dbs;
        double current_current_max_dbs;
        int result_angl_id;
};

