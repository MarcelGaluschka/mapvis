#include "reader.h"



Reader::Reader(const char* filePath, uint16_t BUFFER_SIZE)
{
    int headerSize = sizeof(wav_hdr); 
    int fileLength = 0;

    wav_file = fopen(filePath, "r");
    if(wav_file == nullptr)
    {
        fprintf(stderr, "Unable to open wave file: %s\n", filePath);
        return;
    }
    bytes_read = fread(&wav_header, 1, headerSize, wav_file);

    if(bytes_read > 0)
    {
        bytes_per_sample = wav_header.bitsPerSample / 8;
        num_samples = wav_header.SubChunk2Size / bytes_per_sample;
        this->BUFFER_SIZE = BUFFER_SIZE;
    }
}


bool Reader::getNextBuffer(int16_t* *buffer)
{
    // Header said Audiodata ended or next buffer would extend audiosize
    curr_sample += BUFFER_SIZE;
    if (curr_sample > num_samples)
    {
        return false;
    }

    // Return true if there are more bytes and set buffer values to buffer
    return((bytes_read = fread(*buffer, bytes_per_sample, BUFFER_SIZE, wav_file)) > 0);
}





int Reader::getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}