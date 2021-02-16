#include "csv_writer.h"


void write_csv(std::string filename, std::vector<double> angls, std::vector<double> dbs)
{
    std::ofstream outputFileAngls("output_angls.csv");
    std::ofstream outputFileDbs("output_dbs.csv");

    for (int i = 0; i < angls.size(); i++)
    {
        outputFileAngls << angls.at(i)  << "\n";
        outputFileDbs   << dbs.at(i)    << "\n";
    }

    outputFileAngls.close();
    outputFileDbs.close();
}