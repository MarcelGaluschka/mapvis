#include <string>
#include <fstream> 
#include <vector>
#include <array>

void write_csv(std::string filename, std::vector<std::array<double,2>> angls, std::vector<double> dbs);