#include "global.hpp"

#include <iomanip>

void generateMapFromOriginPtn(bool save_cent_ptn=false) {
  origin_map.clear();
  center_ptn.clear();
  rectype index = 0, temp,
          current_num,
          current_start = 0;
  current_num = getOriginTop(origin_ptn[0]);
#ifdef DEBUG
  std::cout << "All map got from pattern: (num, pos, size)" << std::endl;
#endif
  for (auto &num: origin_ptn) {
    if (getOriginTop(num) != current_num) {
#ifdef DEBUG
      std::cout <<
        std::setw(10) << current_num <<
        std::setw(10) << current_start <<
        std::setw(10) << (index - current_start) << std::endl;
#endif
      temp = (index - current_start)
        | current_start << map_pos_offset;
      origin_map.insert({current_num, temp});
      if (save_cent_ptn) {
        center_ptn.push_back(current_num);
      }
      current_num = getOriginTop(num);
      current_start = index;
    }
    index++;
  }
#ifdef DEBUG
  std::cout <<
    std::setw(10) << current_num <<
    std::setw(10) << current_start <<
    std::setw(10) << (index - current_start) << std::endl;
#endif
  temp = (index - current_start)
    | current_start << map_pos_offset;
  origin_map.insert({current_num, temp});
  if (save_cent_ptn) {
    center_ptn.push_back(current_num);
  }

}

rectype flipCenterNum(rectype num) {
  int center_sq = center_size * center_size;
  rectype cursor = 01ull << (center_flip_coors.size() - 1);
  rectype newnum = 0;
  rectype arr[center_sq];
  std::fill(arr, arr+center_sq, 0);
  for (auto &i: center_flip_coors) {
    arr[i] = 1 && (num & cursor);
    cursor >>= 1;
  }
  for (auto &i: center_coors) {
    newnum <<= 1;
    newnum |= arr[i];
  }
#ifdef DEBUG
  std::cout << "flipping number from: " <<
    num << ", to target num: " << newnum << std::endl;
  std::cout << "The matrix used for the flipping";
  for (int i=0; i<center_sq; i++) {
    if (!(i%center_size)) { std::cout << std::endl; }
    std::cout << std::setw(2) << arr[i];
  }
  std::cout << std::endl;
#endif

  return newnum;
}

void getMappedPtnsStrips(rectype num) {
  origin_b_strides.clear();
  origin_l_strides.clear();
  rectype temp, pos, size;
  temp = origin_map.at(num);
  pos = temp >> map_pos_offset,
  size = temp & map_offset_mask;
  getpartialptnsfromfile(pos, size);
  origin_b_strides.resize(size);
  std::transform(
      get_ptn.begin(), get_ptn.end(),
      origin_b_strides.begin(), getOriginBottom);

  num = flipCenterNum(num);
  temp = origin_map.at(num);
  pos = temp >> map_pos_offset,
  size = temp & map_offset_mask;
  getpartialptnsfromfile(pos, size);
  origin_l_strides.resize(size);
  std::transform(
      get_ptn.begin(), get_ptn.end(),
      origin_l_strides.begin(), getOriginBottom);

#ifdef DEBUG
  std::cout << "origin_b_strides: " <<
    json(origin_b_strides) << std::endl;
  std::cout << "origin_l_strides: " <<
    json(origin_l_strides) << std::endl;
#endif
}


