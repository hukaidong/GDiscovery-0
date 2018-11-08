#include "json.hpp"
#include "slice1.cpp"
#include "slice2.cpp"
#include "slice3.cpp"

using json = nlohmann::json;


int main() {
  init_target_size(5);
  std::cout << __LINE__ << std::endl;
  init_coors();
  std::cout << __LINE__ << std::endl;
  getptnsfromfile();
  std::cout << __LINE__ << std::endl;
  generateMapFromOriginPtn();
  std::cout << __LINE__ << std::endl;
  for (auto &i: origin_map) {
    getMappedPtnsStrips(i.first);
    setTargetArrayToNumByCoor(i.first, center_trangle_coors);
    for (auto &l: origin_l_strides)
    for (auto &b: origin_b_strides) {
      setTargetArrayToNumByCoor(l, left_origin_in_target_coors_extra);
      setTargetArrayToNumByCoor(b, bottom_origin_in_target_coors_extra);
      std::cout << __LINE__ << std::endl;
      showTargetArrays();
    }
  }
  std::cout << __LINE__ << std::endl;

  std::cout << (json)origin_coors << std::endl;
}
