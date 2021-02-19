#include "sod_3d.h"

SOD_3D::SOD_3D(int samplerate, uint16_t BUFFER_SIZE, int angles_x, int angles_y)
{
    this->BUFFER_SIZE = BUFFER_SIZE;
    this->num_mics = 8;

    std::complex<double> imaginary(0.0,1.0);
    mics.resize(num_mics, std::complex<double>());
    for (int i = 0; i < mics.size(); i++)
    {
        mics[i] = 10.0 * exp((imaginary * (double) i / (double) num_mics) * 2.0 * M_PI) / 100.0;
    }

    this->samplerate = (double) samplerate;
    this->k = (int) (BUFFER_SIZE - windowsize) / (windowsize/2.0);
    hamming_array.resize(windowsize, 0.0);
    for (int i = 0; i < hamming_array.size(); i++)
    {
        hamming_array.at(i) = hamming(windowsize, i);
    }

    l_array.resize(windowsize, 0.0);
    inc_matrix.resize(num_mics, 0.0);
    inc_matrix2.resize(num_mics, 0.0);

    this->angles_x = angles_x;
    for (int i = 0; i < angles_x; i++)
    {
        act_angles_x.push_back( i * (360.0/(double)angles_x));
    }
    this->angles_y = angles_y;
    for (int i = 0; i < angles_y; i++)
    {
        act_angles_y.push_back( i * (60.0/(double)angles_y));
    }



    temp_xa.resize(windowsize, 0.0);
    temp_xa_comp.resize(windowsize, std::complex<double> (0.0,0.0));

    temp_y.resize(BUFFER_SIZE, 0.0);

    xa.resize(num_mics, temp_xa);
    ya.resize(num_mics, temp_xa_comp);
    y.resize(num_mics, temp_y);

    std::vector<std::complex<double>> H_x;
    H_x.resize((int) windowsize, std::complex<double>());

    H.resize(num_mics, H_x);

    temp_steering_block.resize(BUFFER_SIZE, 0.0);
    temp_temp_steering_block.resize(angles_y, temp_steering_block);

    current_sum = 0.0;
}

std::array<double,3> SOD_3D::compute(std::array<int16_t*,8> *buffers)
{
    std::complex<double> imaginary(0.0,1.0);
    // reset steering block
    steering_block.resize(angles_x, temp_temp_steering_block);

    double angl_rad;
    double angl_rad_y;
    for (int curr_angl_id = 0; curr_angl_id < angles_x; curr_angl_id++)
    {
        angl_rad = (act_angles_x.at(curr_angl_id)/180) * M_PI;

        for (int curr_angl_y_id = 0; curr_angl_y_id < angles_y; curr_angl_y_id++)
        {

            angl_rad_y = (act_angles_y.at(curr_angl_y_id)/180) * M_PI;
            incoming_ray = {-cos(angl_rad) * cos(angl_rad_y), -sin(angl_rad)* cos(angl_rad_y), -sin(angl_rad_y)};

            for (int i = 0; i < num_mics; i++)
            {
                inc_matrix.at(i) = (real(mics.at(i)) * incoming_ray[0]) / airspeed;
                inc_matrix.at(i) += (imag(mics.at(i)) * incoming_ray[1]) / airspeed;
                inc_matrix.at(i) += (0 * incoming_ray[2]) / airspeed;
                inc_matrix2.at(i) = inc_matrix.at(i) * samplerate;
            }

            for (int k = 0; k < (int) windowsize; k++)
            {
                Th.push_back(2 * M_PI * (k - (windowsize/2))/windowsize);
            }


            for (int j = 0; j < (int) windowsize; j++)
            {
                H.at(0).at(j) = std::complex<double> (1,0);
            }
            for (int i = 1; i < num_mics; i++)
            {
                for (int j = 0; j < (int) windowsize; j++)
                {
                    H.at(i).at(j) = exp(- imaginary * Th.at(j) * inc_matrix2.at(i));
                }
            }
            // TODO: fft shift on h is this correct???
            for (int i = 0; i < num_mics; i++)
            {
                std::rotate(H.at(i).begin(), (H.at(i).begin() + ((int)windowsize >> 1)), H.at(i).end());
            }


            for(int curr_k = 0; curr_k < k; curr_k++)
            {
                // reuse l_array for each k
                for(int i = 0; i < windowsize; i++)
                {
                    l_array.at(i) = (int)(curr_k * (windowsize/2.0) + i);
                }

                for(int m = 0; m < num_mics; m++)
                {
                    for(int j = 0; j < windowsize; j++)
                    {
                        xa.at(m).at(j) = buffers->at(m)[(int) l_array.at(j)];
                    }
                }
                // elementwise mult
                for (int matrix_i = 0; matrix_i < num_mics; matrix_i++)
                {
                    for(int matrix_j = 0; matrix_j < windowsize; matrix_j++)
                    {
                        ya.at(matrix_i).at(matrix_j) = (std::complex<double>)  xa.at(matrix_i).at(matrix_j) * hamming_array.at(matrix_i);
                    }
                }


                for (int i = 0; i < num_mics; i++)
                {
                    fft(ya.at(i));
                }

                for (int i = 0; i < num_mics; i++)
                {
                    for (int j = 0; j < windowsize; j++)
                    {
                        ya.at(i).at(j) *= H.at(i).at(j);
                    }
                }

                for (int i = 0; i < num_mics; i++)
                {
                    ifft(ya.at(i));
                }
                for (int i = 0; i < num_mics; i++)
                {
                    for (int j = 0; j < BUFFER_SIZE; j++)
                    {
                        y.at(i).at(j) = 0.0;
                    }
                }
                for (int i = 0; i < num_mics; i++)
                {
                    for (int j = 0; j < windowsize; j++)
                    {
                        y.at(i).at((int) l_array.at(j)) +=  real(ya.at(i).at(j));
                    }
                }
            } // end for k

            for (int buff_counter = 0; buff_counter < BUFFER_SIZE; buff_counter++)
            {
                current_sum = 0.0;
                for (int curr_channel = 0; curr_channel < num_mics; curr_channel++)
                {
                    current_sum += y.at(curr_channel).at(buff_counter);
                }
                steering_block.at(curr_angl_id).at(curr_angl_y_id).at(buff_counter) = pow(current_sum, 2);
            }

        }
    } // end for each angle

    current_max_angl_x = 0.0;
    current_max_angl_y = 0.0;
    current_max_dbs = 0.0;
    for (int i = 0; i < angles_x; i++)
    {
        for(int j = 0; j < angles_y; j++)
        {
        for (int b = 0; b < BUFFER_SIZE; b++)
            {
                if(20 * log10(steering_block.at(i).at(j).at(b) + (float) 1e-16) > current_max_dbs)
                {
                    current_max_dbs = 20 * log10(steering_block.at(i).at(j).at(b) + (float) 1e-16);
                    current_max_angl_x = act_angles_x.at(i);
                    current_max_angl_y = act_angles_y.at(j);
                }
            }
        }
    }
    return std::array<double,3> {current_max_angl_x, current_max_angl_y, current_max_dbs};

}

double SOD_3D::hamming(double windowsize, int pos)
{
    return 0.54 - 0.46 * cos((2 * M_PI * pos)/(windowsize - 1));
}

void SOD_3D::fft(std::vector<std::complex<double>> &x)
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

void SOD_3D::ifft(std::vector<std::complex<double>> &x)
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