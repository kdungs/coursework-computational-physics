#include <iostream>
#include <sstream>
#include <fstream>

#include "Forest.h"

int main(int argc, char* argv[]){
    std::random_device rnd_device;
    std::mt19937 rng(rnd_device());
    std::unique_ptr<Forest> forest(new Forest(1, 200));
    std::stringstream filename;
       
    unsigned long counter,
                  nPercolations(1000);

    for(size_t L : {5, 10, 20, 50, 100, 200}){
      std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
      forest->Resize(L);
      counter = 0;
      filename << "./data/fit/" << L << ".txt";
      filename.flush();
      std::ofstream file(filename.str());
      for(double p=0.3; p<=0.7; p+=0.01){
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
      std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
      std::cout << "This one (L=" << L <<", " << 40 * nPercolations << " steps) took "
          << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
          << " ms." << std::endl;
    }

    return 0;
}

