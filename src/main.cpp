#include <iostream>

int main(int argc, char *argv[]){
    if(argc == 1)
    {
        std::cout << "No parameter given" << std::endl;
        return 0.0;
    }
    std::cout << argv[1] << std::endl;

    return 0.0;
}