#include <iostream>
#include <vector>
#include <array>


#include <boost/lexical_cast.hpp>

#include <sndfile.h>

#include "wav_reader/reader.h"
#include "doa_compute/sod.h"

int main(int argc, char *argv[]){

    // read arguments
    if(argc < 2)
    {
        std::cout << "No parameter given" << std::endl;
        return 0.0;
    }

    // TODO variable Blocksize
    const uint16_t BUFFER_SIZE = (uint16_t) atoi(argv[1]);
    //const uint16_t BUFFER_SIZE = (uint16_t) 2048;

    const int angles_x = 16;


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

    bool next_buffers_full = true;
    for (int n = 0; n < num_mics; n++)
    {
        if(!readers[n].getNextBuffer(&buffers[n]))
        {
            next_buffers_full = false;
        }
    }


    SOD sod (samplerate, BUFFER_SIZE, angles_x);

    int counter = 0;

    while(next_buffers_full)
    {
        //std::cout << buffers[0][0]  << " " << buffers[1][0]<< std::endl;
        sod.compute(&buffers);



        counter++;
        std::cout << counter << std::endl;
        //if (counter >= 10)
        //{
        //    return 0.0;
        //}



        // get new buffers for next iteration
        for (int n = 0; n < num_mics; n++)
        {
            if(!readers[n].getNextBuffer(&buffers[n]))
            {
                next_buffers_full = false;
            }
        }
    }



    //delete [] buffers;
    //buffers = nullptr;



    return 0.0;
}
