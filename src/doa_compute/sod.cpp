#include "sod.h"

SOD::SOD(int samplerate, uint16_t BUFFER_SIZE)
{
    std::complex<double> imaginary (0,1);
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
    //for (int i = 0; i < mics.size(); i++)
    //{
    //    std::cout << mics[i] << std::endl;
    //}

}

void SOD::compute(std::array<int16_t*,8> *buffers)
{
    //std::cout << buffers->at(0)[0] << " " << buffers->at(1)[0] << std::endl;

}

double SOD::hamming(double windowsize, int pos)
{
    return 0.54 - 0.46 * cos((2 * M_PI * pos)/windowsize - 1);
}