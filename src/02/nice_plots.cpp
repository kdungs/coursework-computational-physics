
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>

#include "Forest.h"

int main(int argc, char* argv[]){
    std::random_device rnd_device;
    std::mt19937 rng(rnd_device());
    std::unique_ptr<Forest> forest(new Forest(5, 200));
    std::stringstream filename;
    for(size_t L : {10, 50, 100}){
      for(double p=0.1; p<=0.9; p+=0.1){
          forest->Resize(L);
          forest->Fill(p, rng);
          filename << "./data/plots/" << L << "-" << p << ".txt";
          filename.flush();
          forest->FindClusters();
          forest->Write(filename.str());
          filename.str(std::string());
      }
    }
    return 0;
}
   
   
