#include <iostream>
#include <random>
#include <string>

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " array size\n";
        return 1;
    }

    int n = std::stoi(argv[1]);

    std::cout << n << "\t";
    for(int i=0; i<n; i++)
    {
        std::cout << i << "\t";
    }
    
    return 0;
}