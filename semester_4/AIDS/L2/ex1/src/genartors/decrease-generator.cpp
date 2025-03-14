#include <iostream>
#include <random>
#include <string>

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " array size\n";
        return 1;
    }

    int n = std::stoi(argv[1]);

    for(int i=n; i>0; i--)
    {
        std::cout << i << "\t";
    }
    
    return 0;
}