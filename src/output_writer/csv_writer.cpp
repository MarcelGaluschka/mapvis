#include "csv_writer.h"


void write_csv(std::string filename, std::vector<std::array<double,2>> angls, std::vector<double> dbs)
{
    std::ofstream outputFileAngls("output_angls.csv");
    std::ofstream outputFileDbs("output_dbs.csv");

    for (int i = 0; i < angls.size(); i++)
    {
        outputFileAngls << angls.at(i)[0]  << "," << angls.at(i)[1] << "\n";
        outputFileDbs   << dbs.at(i)    << "\n";
    }

    outputFileAngls.close();
    outputFileDbs.close();
}