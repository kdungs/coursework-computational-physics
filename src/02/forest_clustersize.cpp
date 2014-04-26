#include <iostream>
#include <sstream>
#include <fstream>

#include "Forest.h"

int main(int argc, char* argv[]){
    std::random_device rnd_device;
    std::mt19937 rng(rnd_device());
    std::unique_ptr<Forest> forest(new Forest(1, 300));
    std::stringstream filename;
       
    for(size_t L : {50, 100, 200, 300}){
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
      forest->Resize(L);
      filename << "./data/clustersize/" << L << ".txt";
      filename.flush();
      std::ofstream file(filename.str());
      for(double p=0.1; p<=0.9; p+=0.05){
        forest->Fill(p, rng);
        file << p << "\t" << forest->biggestCluster() << std::endl;
      }
      file.flush();
      file.close();
      std::cout << filename.str() << " written." << std::endl;
      filename.str(std::string());
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      std::cout << "This one (L=" << L << ") took "
          << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
          << " ms." << std::endl;
    }

    return 0;
}

