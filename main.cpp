#include "slice1.cpp"
#include "slice2.cpp"
#include "slice3.cpp"

int main(int argc, const char* argv[]) {
  if (argc != 2) { std::cerr << "Unsupported arguments" << std::endl; exit(1);}
  init_target_size(std::stoi(argv[1]));
  init_coors();
  getptnsfromfile();
  generateMapFromOriginPtn();
  reset_arr();
  for (auto &i: origin_map) {
    getMappedPtnsStrips(i.first);
    setTargetArrayToNumByCoor(i.first, center_trangle_coors);
    for (auto &l: origin_l_strides)
    for (auto &b: origin_b_strides) {
      setTargetArrayToNumByCoor(l, left_origin_in_target_coors_rextra);
      setTargetArrayToNumByCoor(b, bottom_origin_in_target_coors_extra);
      verify_avail_target_ptn();
    }
  }
  saveptnstofile();
}
