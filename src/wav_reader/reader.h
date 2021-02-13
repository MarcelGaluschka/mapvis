#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;

typedef struct  WAV_HEADER
{
    /* RIFF Chunk */
    uint8_t         RIFF[4];        // RIFF Header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* fmt sub-chunk */
    uint8_t         fmt[4];         // FMT Header
    uint32_t        SubChunk1Size;  // Size of FMT Chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         SubChunk2ID[4]; // "data"  string
    uint32_t        SubChunk2Size;  // Sampled data length
} wav_hdr;



class Reader
{
    public:
    Reader(const char* filePath);
    ~Reader(){fclose(wav_file);}

    int getFileSize(FILE* inFile);

    bool getNextBuffer(int16_t* *buffer);

    int readFile(const char* filePath);


    protected:
        wav_hdr wav_header;

        FILE* wav_file;
        size_t bytes_read;
    private: 
        uint16_t bytes_per_sample;
        uint64_t num_samples;
        uint16_t BUFFER_SIZE;
        //int16_t* buffer;
        uint64_t curr_sample = 0;



};

