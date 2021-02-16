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
        const int num_mics = 8;
        double samplerate;
        int k;

        int angles_x;
        std::vector<double> act_angles_x;

        const double windowsize = 512;
        std::array<double, 512> hamming_array;
        const double airspeed = 343;
        uint16_t BUFFER_SIZE;

        std::vector<std::vector<double>> xa;
        std::vector<std::vector<double>> Xa_temp;
        std::vector<std::vector<double>> Xa;
        std::vector<std::vector<std::complex<double>>> Ya;
        std::vector<std::vector<std::complex<double>>> ya;
        std::vector<std::vector<double>> y;

        std::vector<double> temp_xa;
        std::vector<std::complex<double>> temp_xa_comp;
        std::vector<double> temp_y;

        std::array<double, 512> l_array;

        std::array<double,2> incoming_ray;

        std::array<std::array<std::complex<double>,2>,8> inc_matrix;
        std::array<std::array<std::complex<double>,2>,8> inc_matrix2;

        std::array<std::array<std::array<std::complex<double>,2>,512>,8> H;
        std::vector<double> Th;


        std::vector<double> temp_steering_block;
        std::vector<std::vector<double>> steering_block;
};

