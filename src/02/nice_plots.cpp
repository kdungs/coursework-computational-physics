
#include <iostream>
#include <sstream>
#include <fstream>

#include "Forest.h"

int main(int argc, char* argv[]){
    std::random_device rnd_device;
    std::mt19937 rng(rnd_device());
    Forest *forest = new Forest(1, 200);
    std::stringstream filename;
    for(int L=5; L<=30; L+=5){
      for(double p=0.1; p<=0.9; p+=0.1){
          forest->Resize(L);
          forest->Fill(p, rng);
          filename << "./data/" << L << "-" << p << ".txt";
          filename.flush();
          forest->FindClusters();
          forest->Write(filename.str());
          filename.str(std::string());
      }
    }
    delete forest;
    return 0;
}
   
   
