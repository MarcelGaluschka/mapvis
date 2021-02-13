#include "reader.h"



Reader::Reader(const char* filePath)
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
        BUFFER_SIZE = 16;
        //buffer = new int16_t[BUFFER_SIZE];
    }
}


bool Reader::getNextBuffer(int16_t* *buffer)
{
    cout << curr_sample << endl;
    cout << num_samples << endl;
    cout << (curr_sample == num_samples) << endl;
    if ((curr_sample == num_samples) > 0);
    {
        cout << "true" << endl;
        return false;
    }
    curr_sample++; 
    return((bytes_read = fread(*buffer, bytes_per_sample, BUFFER_SIZE, wav_file)) > 0);
}


int Reader::readFile(const char* filePath)
{
    wav_hdr wavHeader;
    int headerSize = sizeof(wav_hdr), filelength = 0;
    cout << headerSize << endl;


    FILE* wavFile = fopen(filePath, "r");
    if (wavFile == nullptr)
    {
        fprintf(stderr, "Unable to open wave file: %s\n", filePath);
        return 1;
    }

    // Read the header
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    cout << "Header Read " << bytesRead << " bytes." << endl;

    if(bytesRead > 0)
    {
        //Read the headerData
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;
        uint64_t numSamples = wavHeader.SubChunk2Size / bytesPerSample;
        static const uint16_t BUFFER_SIZE = 16;
        int16_t* buffer = new int16_t[BUFFER_SIZE];
        const float max_sample =  (2 << wavHeader.bitsPerSample-2);

        uint64_t currSample = 0;


        while((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile))>0)
        {

            for (unsigned int i = 0; i < BUFFER_SIZE; i+=2)
            {
                buffer[i];
                currSample++;
                if (currSample == numSamples)
                {
                    delete [] buffer;
                    buffer = nullptr;
                    return 0;
                }
            }

        }

        cout << bytesPerSample << endl;
        cout << sizeof(buffer[0]) << endl;


        delete [] buffer;
        buffer = nullptr;

    }
    fclose(wavFile);


    return 0;
}



int Reader::getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}