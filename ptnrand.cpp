#include "global.hpp"
#include "slice1.cpp"
#include "slice2.cpp"
#include "slice3.cpp"

#include <ctime>
#include <cstdlib>

int main(int argc, const char* argv[]) {
  std::srand(std::time(0));
  if (argc != 2) { std::cerr << "Unsupported arguments" << std::endl; exit(1);}
  std::cout << "Initializing..." << std::endl;
  init_target_size(std::stoi(argv[1]));
  init_coors();
  getptnsfromfile();
  generateMapFromOriginPtn(true);
  reset_arr();
  target_file += ".rnd";

  int rnd_idx = center_ptn.size() * std::rand() / RAND_MAX;
  std::cout << "Center pattern size: " << center_ptn.size() << std::endl;
  std::cout << "Picked random index: " << rnd_idx << std::endl;
  rectype i = center_ptn.at(rnd_idx);
  getMappedPtnsStrips(i);
  setTargetArrayToNumByCoor(i, center_trangle_coors);

  for (auto &l: origin_l_strides)
  for (auto &b: origin_b_strides) {
    setTargetArrayToNumByCoor(l, left_origin_in_target_coors_rextra);
    setTargetArrayToNumByCoor(b, bottom_origin_in_target_coors_extra);
    verify_avail_target_ptn();
  }

  show_at_index_enabled = true;
  show_at_index = target_ptn.size() * std::rand() / RAND_MAX;
  std::cout << "Generated " << target_ptn.size() << " patterns" << std::endl;
  std::cout << "Randomly choose index " << show_at_index << " pattern" << std::endl;

  for (auto &l: origin_l_strides)
  for (auto &b: origin_b_strides) {
    setTargetArrayToNumByCoor(l, left_origin_in_target_coors_rextra);
    setTargetArrayToNumByCoor(b, bottom_origin_in_target_coors_extra);
    verify_avail_target_ptn();
  }
}
