#include <iostream>
#include <sstream>
#include <fstream>

#include "Forest.h"

int main(int argc, char* argv[]){
    std::random_device rnd_device;
    std::mt19937 rng(rnd_device());
    Forest *forest = new Forest(1, 200);
    std::stringstream filename;
       
    unsigned long counter,
                  nPercolations(100);

    for(size_t L : {4, 6, 8, 10, 15, 20, 25, 30, 45, 60, 80, 100, 150, 200}){
      forest->Resize(L);
      counter = 0;
      filename << "./data/" << L << ".txt";
      filename.flush();
      std::ofstream file(filename.str());
      for(double p=0.2; p<=0.9; p+=0.01){
        for(int n=0; n<nPercolations; n++){
          forest->Fill(p, rng);
          if(forest->HasPercolation()){
            counter++;
          }
          forest->Clean();
        }
        file << p << "\t" << counter << std::endl;
        counter = 0;
      }
      file.flush();
      file.close();
      std::cout << filename.str() << " written." << std::endl;
      filename.str(std::string());
    }

    delete forest;

    return 0;
}

