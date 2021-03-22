#include <iostream>
#include <vector>
#include <array>

#include <chrono>


#include <boost/lexical_cast.hpp>


#include "iir1/Iir.h"

#include "wav_reader/reader.h"
#include "doa_compute/doa_estimator.h"
#include "doa_compute/sod.h"
#include "doa_compute/sod_3d.h"
//#include "doa_compute/music_3d.h"
#include "output_writer/csv_writer.h"

int main(int argc, char *argv[]){

    // read arguments
    if(argc < 2)
    {
        std::cout << "No parameter given" << std::endl;
        return 0.0;
    }

    const uint16_t BUFFER_SIZE = (uint16_t) atoi(argv[1]);
    //const uint16_t BUFFER_SIZE = (uint16_t) 2048;

    const int angles_x = 16;
    const int angles_y = 3;

    char* filePath = argv[2];
    string filePathString = filePath;
    filePathString.erase(filePathString.size()-5);

    const int num_mics = 8;

    std::array<char*, num_mics> filePaths;
    for (int i = 0; i < filePaths.size(); i++)
    {
        filePaths[i] = strdup((filePathString + boost::lexical_cast<string>(i+1) + ".wav").c_str());
    }


    std::array<Reader, num_mics> readers; 
    for (int i = 0; i < num_mics; i++)
    {
        readers[i].openFile(filePaths[i], BUFFER_SIZE);
    }


    std::array<int16_t* ,num_mics> buffers;
    for (int i = 0; i < num_mics; i++)
    {
        buffers[i] = new int16_t[BUFFER_SIZE];
    }

    uint32_t samplerate = readers[0].getSamplerate();


    // new object of Direction finder
    //SOD sod (samplerate, BUFFER_SIZE, angles_x);
    //SOD_3D sod (samplerate, BUFFER_SIZE, angles_x, angles_y);
    SOD_3D sod (samplerate, BUFFER_SIZE, angles_x, angles_y);
    

    //new object of filter 8th order between 600 and 6000 Hz
    const double broadband_filter_coeff[][6] = {
        {0.05445625, 0.10743313, 0.05445625,  1.0, -0.0950718, 0.4306414},
        {1.0, -1.99776801, 1.0, 1.0, -1.35646634, 0.63553658},
        {      1.        ,  1.8187884 ,  1.        ,  1.,          0.85170717,  0.72936388},
        {      1.        , -1.98456217,  1.        ,  1.,         -1.78771671,  0.88981689},
        {      1.        ,  1.67840562,  1.        ,  1.,          1.24844883,  0.89660922},
        {      1.        , -1.97164923,  1.        ,  1.,         -1.89872608,  0.96371827},
        {      1.        ,  1.61189906,  1.        ,  1.,          1.39397587,  0.97318931},
        {      1.        , -1.9652127 ,  1.        ,  1.,         -1.93591133,  0.99106541}
    };
    const int nSOS = sizeof(broadband_filter_coeff) / sizeof(broadband_filter_coeff[0]);
    Iir::Custom::SOSCascade<nSOS> broadband_filter (broadband_filter_coeff);
    
    //std::complex<double> imaginary (0.0,1.0);
    //double band_width = 1500;
    //double center_frequency = 1250;
    //double thc = (center_frequency * 2 * M_PI) / samplerate;
    //double rho = exp((-band_width * M_PI)/ samplerate);
    //std::complex<double> pl = rho * exp(imaginary * thc);
    //const double narrowband_filter_coeff[][6] = {
    //       {1.0, 0, 0,  1.0, (- 1) *real(pl), 0}
    //   };

    //Iir::Custom::SOSCascade<1> narrowband_filter (narrowband_filter_coeff);

    int counter = 0;

    auto startTime = std::chrono::high_resolution_clock::now();

    std::array<double,3> resultbuffer;
    std::vector<std::array<double,2>> results_angl;
    std::vector<double> results_dbs;

    bool next_buffers_full = true;
    for (int n = 0; n < num_mics; n++)
    {
        if(!readers[n].getNextBuffer(&buffers[n]))
        {
            next_buffers_full = false;
        }
    }
    while(next_buffers_full)
    {
        for (int mic = 0; mic < num_mics; mic++)
        {
            for(int i = 0; i < BUFFER_SIZE; i++)
            {
                buffers[mic][i] = broadband_filter.filter(buffers[mic][i]);
            }
        }

        resultbuffer = sod.compute(&buffers);
        results_angl.push_back(std::array<double,2> {resultbuffer[0], resultbuffer[1]});
        results_dbs.push_back(resultbuffer[2]);

        if(counter % 10 == 0)
        {
            // print something every tenth block to estimate runtime
            std::cout << counter << std::endl;
        }

        // get new buffers for next iteration
        for (int n = 0; n < num_mics; n++)
        {
            if(!readers[n].getNextBuffer(&buffers[n]))
            {
                next_buffers_full = false;
            }
        }
        counter++;
    }

    // stop timer
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timing;
    timing = endTime - startTime;
    std::cout << "Time needed for computation: " <<  timing.count() << " seconds"<< std::endl;

    // output two files for angle and dbs
    write_csv("", results_angl, results_dbs, (int) samplerate, (int) BUFFER_SIZE);


    return 0.0;
}
