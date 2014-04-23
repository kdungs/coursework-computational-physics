/** @file perculation.h
 * Define several functions relevant for simulations concerning perculating
 * clusters.
 */
#ifndef __PERCULATION_H__
#define __PERCULATION_H__

#include <map>
#include <random>
#include <string>
#include <vector>

std::vector<bool> generateGrid(const size_t, const double, std::mt19937&);
std::vector<int> findClusters(const std::vector<bool>&);
int findPerculating(const std::vector<int>&);
std::map<int, size_t> clusterSizes(const std::vector<int>&);
void writeToFile(const std::string&, const std::vector<int>&);

#endif /* __PERCULATION_H__ */
