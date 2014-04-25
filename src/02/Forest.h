#ifndef __FOREST_H__
#define __FOREST_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <random>

typedef unsigned long ClusterSize_t;

class Forest {
  private:
    size_t length_;
    size_t max_length_;
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
            && (newPoint.first < length_) 
            && (newPoint.second >= 0)
            && (newPoint.second < length_)){
          IsCluster(newPoint, size + 1); // bigger cluster!!!
        }
      }
      return true;
    }

    /** Get the size of the given cluster
     * @param clusterID the cluster ID
     * @return the size
     */
    ClusterSize_t ClusterSize(ClusterSize_t clusterID){
      ClusterSize_t size(0);
      std::for_each(std::begin(forest_), std::begin(forest_)+length_,
                    [&](std::vector<ClusterSize_t> &line){
                      std::for_each(std::begin(line), std::begin(line)+length_,
                                    [&](ClusterSize_t &tree){
                                        if(tree == clusterID)
                                            size++;
                      });
      });
      return size;
    }

  public:
    Forest(){};
    /** Constuctor with forest size
     *
     * @param length the length of both forest edges
     * @return a forest without trees
     */
    Forest(size_t length, size_t max_length=100) :
      length_(length),
      max_length_(max_length),
      cluster_number_(1)
    {
      forest_.resize(max_length_);
      for(std::vector<ClusterSize_t> &line : forest_){
        line.resize(max_length_);
        Clean();
      }
    }
    ~Forest(){};

    /** Plant trees
     *
     * @param p the probability for planting a tree
     * @rng any random generator
     */
    void Fill(double p, std::mt19937 &rng){
      Clean();
      static std::uniform_real_distribution<double> dist(0.0, 1.0);
      std::for_each(std::begin(forest_),
                    std::begin(forest_)+length_,
                    [&](std::vector<ClusterSize_t> &line){
                      std::for_each(std::begin(line),
                                    std::begin(line)+length_,
                                    [&](ClusterSize_t &tree){
                                      if(dist(rng) < p){
                                        tree = 1;
                                      }
                      });
      });
    }

    /** cut all trees
     */
    void Clean(){
      cluster_number_ = 1;
      for(std::vector<ClusterSize_t> &line : forest_){
        std::fill(std::begin(line), std::end(line), 0);
      }
    }

    void Resize(size_t length){
      Clean();
      length_ = length;
    }

    /** Search the forest for clusters
     *
     * @return the number of found clusters (single-tree clusters count 1)
     */
    ClusterSize_t FindClusters(){
      for(short x=0; x<length_; x++){
        for(short y=0; y<length_; y++){
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
      for(size_t k=0; k<length_; k++){
        for(size_t i=0; i<length_; i++){
          if(forest_[k][i]){
            ofs << k << "\t" << i << "\t" << forest_[k][i] << std::endl;
          }
        }
      }
      ofs.close();
    }

    /** Check whether there is a percolating Cluster
     *
     * @return haspercolation
     */
    bool HasPercolation(){
      if(cluster_number_ <= 1){
        FindClusters();
      }
      bool hasLeftTree(false),
           hasRightTree(false);
      for(ClusterSize_t &topTree : forest_[0]){
        for(ClusterSize_t &bottomTree : forest_[length_ - 1]){
          if(topTree > 1 && topTree == bottomTree){
            hasLeftTree = false;
            hasRightTree = false;
            for(size_t i=0; i<length_; i++){
              if(forest_[0][i] == topTree)
                hasLeftTree = true;
              if(forest_[length_ - 1][i] == topTree)
                hasRightTree = true;
            }
            if(hasLeftTree && hasRightTree){
              ClusterSize(topTree);
              return true;
            }
          }
        }
      }
      return false;
    }

    /** Get the size of the biggest cluster
     * @return cluster size of largest cluster
     */
    ClusterSize_t biggestCluster(){
        if(cluster_number_ <= 1){
            FindClusters();
        }
        ClusterSize_t maxSize(0),
                      size(0);
        std::for_each(std::begin(forest_), std::begin(forest_) + length_,
                      [&](std::vector<ClusterSize_t> &line){
                         std::for_each(std::begin(line), std::begin(line) + length_,
                                       [&](ClusterSize_t &tree){
                                           if(tree > 1){
                                               size = ClusterSize(tree);
                                               maxSize = size > maxSize ? size : maxSize;
                                           }
                                       });
                      });
        return maxSize;
    }
};

#endif // __FOREST_H__
