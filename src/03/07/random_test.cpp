#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "Randinium.hpp"

const size_t nBins(30);

int main(int argc, char* argv[]){
    Randinium rnd;

    // b)
    std::vector<unsigned long long> bins;
    for(size_t i=0; i<nBins; i++){
        bins.push_back(0);
    }
    double number;
    for(size_t i=0; i<1000000; i++){
        number = rnd.NormRand();
        size_t index = (size_t)(number * (double)nBins);
        bins[index]++;
    }
    std::ofstream ofs("histogram.txt");
    for(size_t i=0; i<nBins; i++){
        ofs << i << "\t" << bins[i] << std::endl;
    }
    ofs.close();

    // c) d)
    std::mt19937 engine(0);
    std::uniform_real_distribution<> dist(0, 1);
    
    // 2d
    ofs.open("2d_dist.txt");
    std::ofstream randOfs("2d_dist_rand.txt");
    std::ofstream mtOfs("2d_dist_mt.txt");
    for(size_t i=0; i<10000; i++){
        ofs << rnd.NormRand() << "\t" << rnd.NormRand() << std::endl;
        randOfs << (double)rand()/(double)RAND_MAX << "\t"
                << (double)rand()/(double)RAND_MAX << std::endl;
        mtOfs << dist(engine) << "\t" << dist(engine) << std::endl;
    }
    mtOfs.close();
    randOfs.close();
    ofs.close();

    // 3d
    ofs.open("3d_dist.txt");
    randOfs.open("3d_dist_rand.txt");
    mtOfs.open("3d_dist_mt.txt");
    for(size_t i=0; i<10000; i++){
        ofs << rnd.NormRand() << "\t" << rnd.NormRand() << "\t" << rnd.NormRand() << std::endl;
        randOfs << (double)rand()/(double)RAND_MAX << "\t"
                << (double)rand()/(double)RAND_MAX << "\t"
                << (double)rand()/(double)RAND_MAX << std::endl;
        mtOfs << dist(engine) << "\t"
              << dist(engine) << "\t"
              << dist(engine) << std::endl;
    }
    mtOfs.close();
    randOfs.close();
    ofs.close();

    return 0;
}

