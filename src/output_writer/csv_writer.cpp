#include "csv_writer.h"


void write_csv(std::string filename, std::vector<std::array<double,2>> angls, std::vector<double> dbs, int samplerate, int blocksize)
{
    std::ofstream outputFile("output_mapvis.csv");

    outputFile << samplerate << "," << blocksize << "\n";

    for (int i = 0; i < angls.size(); i++)
    {
        outputFile << angls.at(i)[0]  << "," << angls.at(i)[1] << "," << dbs.at(i) << "\n";
    }

    outputFile.close();
}