#include "sod.h"

SOD::SOD(int samplerate, uint16_t BUFFER_SIZE, int angles_x)
{
    std::complex<double> imaginary(0.0,1.0);
    for (int i = 0; i < mics.size(); i++)
    {
        mics[i] = 10.0 * exp((imaginary * (double)i / 8.0) * 2.0 * M_PI) / 100.0;
    }
    this->samplerate = (double) samplerate;
    this->k = (int) (BUFFER_SIZE - windowsize) / (windowsize/2.0);
    for (int i = 0; i < hamming_array.size(); i++)
    {
        hamming_array[i] = hamming(windowsize, i);
    }
    for (int i = 0; i < angles_x; i++)
    {
        act_angles_x.push_back( i * (360.0/(double)angles_x));
    }

    //for (int i = 0; i < mics.size(); i++)
    //{
    //    std::cout << mics[i] << std::endl;
    //}

}

void SOD::compute(std::array<int16_t*,8> *buffers)
{
    //std::cout << buffers->at(0)[0] << " " << buffers->at(1)[0] << std::endl;
    std::complex<double> imaginary(0.0,1.0);
    double angl_rad;
    std::array<double,2> incoming_ray;
    for (int i = 0; i < angles_x; i++)
    {
        angl_rad = (act_angles_x.at(i)/180) * M_PI;
        incoming_ray = {-cos(angl_rad), -sin(angl_rad)};
    }

    std::array<std::array<std::complex<double>,2>,8> inc_matrix;
    std::array<std::array<std::complex<double>,2>,8> inc_matrix2;
    for (int i = 0; i < num_mics; i++)
    {
        for (int j = 0; j < incoming_ray.size(); j++)
        {
            inc_matrix[i][j] = (mics[i] * incoming_ray[j]) / airspeed;
            inc_matrix2[i][j] = inc_matrix[i][j] * samplerate;
        }
    }

    std::vector<double> Th;
    for (int k = 0; k < (int) windowsize; k++)
    {
        Th.push_back(2 * M_PI * (k - (windowsize/2))/windowsize);
    }

    std::array<std::array<std::array<std::complex<double>,2>,512>,8> H;
    for (int j = 0; j < (int) windowsize; j++)
    {
        for (int k = 0; k < 1; k++)
        {
            H[0][j][k] = std::complex<double> (1,0);
        }
    }
    for (int i = 1; i < num_mics; i++)
    {
        for (int j = 0; j < (int) windowsize; j++)
        {
            for (int k = 0; k < 1; k++)
            {
                H[i][j][k] = exp(- imaginary * Th.at(j) * inc_matrix2[i][k]);
            }
        }
    }
    // TODO fft shift on h

    const int c_k = k;
    std::array<double, 512> l_array;

    for(int curr_k = 0; curr_k < k; k++)
    {
        // reuse l_array for each k
        for(int i = 0; i < windowsize; i++)
        {
            l_array[i] = (int)(curr_k * (windowsize/2.0) + i);
        }

    }


}

double SOD::hamming(double windowsize, int pos)
{
    return 0.54 - 0.46 * cos((2 * M_PI * pos)/(windowsize - 1));
}