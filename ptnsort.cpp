#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

int main(int argc, const char* argv[]) {
  int data_size, ptn_size;
  std::string ifname, ofname;
  std::vector<u_int64_t> patterns;

  if (argc != 2) { std::cerr << "Unsupported arguments" << std::endl; exit(1);}
  ifname = std::string(argv[1]);
  ofname = ifname + ".orig";

  std::ifstream read(ifname,
      std::ios::in | std::ios::binary | std::ios::ate);

  data_size = read.tellg();
  ptn_size = data_size / sizeof(u_int64_t);
  read.seekg(0);

  patterns.resize(ptn_size);
  read.read((char*)patterns.data(), data_size);

  std::sort(patterns.begin(), patterns.end());

  read.seekg(0);
  std::ofstream write(ofname, std::ios::binary);
  write << read.rdbuf();
  read.close();
  write.close();

  write.open(ifname, std::ios::binary);
  write.write((char*)patterns.data(), data_size);
  write.close();
}
