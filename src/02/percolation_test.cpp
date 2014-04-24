#include <iostream>
#include <sstream>

#include "Forest.h"

int main(int argc, char* argv[]){
    std::random_device rnd_device;
    std::mt19937 rng(rnd_device());
    Forest *forest;
    std::stringstream filename;
    //forest = new Forest(10);
    //forest->Fill(0.6, rng);
    //std::cout << forest->FindClusters() << " Clusters found" << std::endl;
    //forest->Write("./data/10-0.6.txt");
    //delete forest;
    for(double p=0.1; p<=0.9; p+=0.1){
        forest = new Forest(10);
        forest->Fill(p, rng);
        filename << "./data/" << 10 << "-" << p << ".txt";
        filename.flush();
        std::cout << forest->FindClusters() << " Clusters found" << std::endl;
        forest->Write(filename.str());
        filename.str(std::string());
        delete forest;
    }
    return 0;
}

