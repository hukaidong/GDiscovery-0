#include "global.hpp"
#include "slice1.cpp"
#include "slice2.cpp"
#include "slice3.cpp"

#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

int main(int argc, const char* argv[]) {
  rectype data_size, ptn_size, target_size;
  std::srand(std::time(0));
  std::string ifname;
  std::vector<rectype> patterns;

  if (argc != 3) { std::cerr << "Unsupported arguments" << std::endl; exit(1);}
  ifname = std::string(argv[1]);
  target_size = std::stoi(argv[2]);

  std::ifstream read(ifname,
      std::ios::in | std::ios::binary | std::ios::ate);

  data_size = read.tellg();
  ptn_size = data_size / sizeof(rectype);
  read.seekg(0);

  patterns.resize(ptn_size);
  read.read((char*)patterns.data(), data_size);
  init_target_size(target_size);
  init_coors();
  reset_arr();
  int rnd_idx = patterns.size() * std::rand() / RAND_MAX;
  std::cout << "Size of pattern: " << patterns.size() << std::endl;
  std::cout << "Random index: " << rnd_idx << std::endl;

  rectype ptn = patterns.at(rnd_idx);
  setTargetArrayToNumByCoor(ptn, target_coors);
  showTargetArrays();
}
