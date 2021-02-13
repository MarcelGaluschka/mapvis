#include <iostream>
#include <vector>

#include <sndfile.h>

#include "wav_reader/reader.h"

int main(int argc, char *argv[]){

    // read arguments
    if(argc < 2)
    {
        std::cout << "No parameter given" << std::endl;
        return 0.0;
    }

    uint16_t BUFFER_SIZE = (uint16_t) atoi(argv[1]);
    const char* filePath = argv[2];
    std::cout << BUFFER_SIZE << "\t" << filePath << std::endl;
    // print arguments


    Reader reader(filePath, BUFFER_SIZE);


    int16_t* buffer = new int16_t[BUFFER_SIZE];
    while(reader.getNextBuffer(&buffer))
    {
        for (unsigned int i = 0; i < BUFFER_SIZE; i++)
        {
        }
    }

    delete [] buffer;
    buffer = nullptr;

    return 0.0;
}