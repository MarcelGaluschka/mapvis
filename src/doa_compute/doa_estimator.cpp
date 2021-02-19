#include "doa_estimator.h"


double DOA_EST::hamming(double windowsize, int pos)
{
    return 0.54 - 0.46 * cos((2 * M_PI * pos)/(windowsize - 1));
}

void DOA_EST::fft(std::vector<std::complex<double>> &x)
{
     // DFT
    unsigned int N = x.size();
    unsigned int k = N;
    unsigned int n;
    double thetaT = 3.14159265358979323846264338328L / N;
    std::complex<double> phiT = std::complex<double> (cos(thetaT), -sin(thetaT));
    std::complex<double> T;
    while (k > 1)
    {
        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        T = 1.0L;
        for (unsigned int l = 0; l < k; l++)
        {
            for (unsigned int a = l; a < N; a += n)
            {
                unsigned int b = a + k;
                std::complex<double> t = x.at(a) - x.at(b);
                x.at(a) += x.at(b);
                x.at(b) = t * T;
            }
            T *= phiT;
        }
    }
    // Decimate
    unsigned int m = (unsigned int)log2(N);
    for (unsigned int a = 0; a < N; a++)
    {
        unsigned int b = a;
        // Reverse bits
        b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
        b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
        b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
        b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
        if (b > a)
        {
            std::complex<double> t = x.at(a);
            x.at(a) = x.at(b);
            x.at(b) = t;
        }
    }

    //for (int i = 0; i < x.size(); i++)
    //{
    //    x.at(i) /= x.size();
    //}

    return;
}

void DOA_EST::ifft(std::vector<std::complex<double>> &x)
{
    for (int i = 0; i < x.size(); i++)
    {
        x.at(i) = std::conj(x.at(i));
    }

    fft(x);

    for (int i = 0; i < x.size(); i++)
    {
        x.at(i) = std::conj(x.at(i));
    }

    for (int i = 0; i < x.size(); i++)
    {
        x.at(i) /= x.size();
    }

}