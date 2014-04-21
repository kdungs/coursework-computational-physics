#ifndef __FOREST_H__
#define __FOREST_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <random>

typedef unsigned short ClusterSize_t;

class Forest {
  private:
    std::vector<std::vector<ClusterSize_t>> forest_;
    ClusterSize_t cluster_number_;
    
    /** Find clusters.
     * All connected trees will get a number > 1
     *
     * @param point Check this tree for membership of any cluster
     * @param size depth of the recursion
     * @return true if the tree belongs to a cluster with size > 1
     */
    bool IsCluster(std::pair<short, short> point, ClusterSize_t size=0){
      if(forest_[point.first][point.second] != 1) // if there's no tree, there's no cluster
        return false;
      if(size){ // this cluster has size>1 -> write cluster_number_ to the forest
        forest_[point.first][point.second] = cluster_number_;
      } else { // this is a new cluster, so increase cluster_number_
        ++cluster_number_;
      }
      std::vector<std::pair<short, short>>  directions{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
      for(std::pair<short, short> direction : directions){
        std::pair<short, short> newPoint(
            point.first + direction.first,
            point.second + direction.second);
        if((newPoint.first >= 0) // check for range violations
            && (newPoint.first < forest_.size()) 
            && (newPoint.second >= 0)
            && (newPoint.second < forest_.size())){
          IsCluster(newPoint, size + 1); // bigger cluster!!!
        }
      }
      return true;
    }

  public:
    Forest(){};
    /** Constuctor with forest size
     *
     * @param length the length of both forest edges
     * @return a forest without trees
     */
    Forest(size_t length) :
      cluster_number_(1)
    {
      forest_.resize(length);
      for(std::vector<ClusterSize_t> &line : forest_){
        line.resize(length);
        std::fill(std::begin(line), std::end(line), 0);
      }
    }
    ~Forest(){};

    /** Plant trees
     *
     * @param p the probability for planting a tree
     * @rng any random generator
     */
    void Fill(double p, std::mt19937 &rng){
      static std::uniform_real_distribution<double> dist(0.0, 1.0);
      for(std::vector<ClusterSize_t> &line : forest_){
        for(ClusterSize_t &tree : line){
          if(dist(rng) < p){
            tree = 1;
          }
        }
      }
    }

    /** Search the forest for clusters
     *
     * @return the number of found clusters (single-tree clusters count 1)
     */
    ClusterSize_t FindClusters(){
      for(short x=0; x<forest_.size(); x++){
        for(short y=0; y<forest_[x].size(); y++){
          IsCluster(std::pair<short, short>(x, y));
        }
      }
      return cluster_number_ - 1;
    }

    /* Print the forest on the console.
     * ugly output :P
     */
    void Print(){
      for(std::vector<ClusterSize_t> &line : forest_){
        for(ClusterSize_t &tree : line){
          std::cout << tree << " ";
        }
        std::cout << std::endl;
      }
    }

    /** Write forest to file.
     * x, y coord and cluster number, seperated by tabs
     *
     * @param filename the filename
     */
    void Write(const std::string filename){
      std::ofstream ofs(filename);
      for(size_t k=0; k<forest_.size(); k++){
        for(size_t i=0; i<forest_[k].size(); i++){
          if(forest_[k][i]){
            ofs << k << "\t" << i << "\t" << forest_[k][i] << std::endl;
          }
        }
      }
      ofs.close();
    }
};

#endif // __FOREST_H__
