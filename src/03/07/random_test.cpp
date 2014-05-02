#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "Randinium.hpp"


int main(int argc, char* argv[]){
    Randinium rnd;

    // b)
    std::ofstream ofs("data/histogram.txt");
    for(size_t i=0; i<1000000; i++){
        ofs << rnd.NormRand() << std::endl;
    }
    ofs.close();

    // c) d)
    std::mt19937 engine(0);
    std::uniform_real_distribution<> dist(0, 1);
    
    // 2d
    ofs.open("data/2d_dist.txt");
    std::ofstream randOfs("data/2d_dist_rand.txt");
    std::ofstream mtOfs("data/2d_dist_mt.txt");
    for(size_t i=0; i<1000; i++){
        ofs << rnd.NormRand() << "\t" << rnd.NormRand() << std::endl;
        randOfs << (double)rand()/(double)RAND_MAX << "\t"
                << (double)rand()/(double)RAND_MAX << std::endl;
        mtOfs << dist(engine) << "\t" << dist(engine) << std::endl;
    }
    mtOfs.close();
    randOfs.close();
    ofs.close();

    // 3d
    ofs.open("data/3d_dist.txt");
    randOfs.open("data/3d_dist_rand.txt");
    mtOfs.open("data/3d_dist_mt.txt");
    for(size_t i=0; i<1000; i++){
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

