#include <iostream>

#include <sndfile.h>

#include "wav_reader/reader.h"

int main(int argc, char *argv[]){



    if(argc == 1)
    {
        std::cout << "No parameter given" << std::endl;
        return 0.0;
    }
    std::cout << argv[1] << std::endl;

    Reader reader(argv[1]);

    int16_t minval = 0;
    int16_t maxval = 0;


    int16_t* buffer = new int16_t[16];
    while(reader.getNextBuffer(&buffer))
    {
        for (unsigned int i = 0; i < 16; i++)
        {
            if(buffer[i] < minval)
            {
                minval = buffer[i];
            }
            if(buffer[i]  > maxval)
            {
                maxval = buffer[i];
            }
        }
    }
    std::cout << maxval << std::endl;
    std::cout << minval << std::endl;

    delete [] buffer;
    buffer = nullptr;



    //readFile(argv[1]);

    return 0.0;
}