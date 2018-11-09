#include "global.hpp"


void generateMapFromOriginPtn() {
  origin_map.clear();
  rectype index = 0, temp,
          current_num,
          current_start = 0;
  current_num = getOriginTop(origin_ptn[0]);
  for (auto &num: origin_ptn) {
    if (getOriginTop(num) != current_num) {
      temp = (index - current_start)
        | current_start << map_pos_offset;
      origin_map.insert({current_num, temp});
      current_num = num;
      current_start = index;
    }
    index++;
  }
  temp = (index - current_start)
    | current_start << map_pos_offset;
  origin_map.insert({current_num, temp});
}

rectype flipCenterNum(rectype num) {
  rectype cursor = 01ull << (center_flip_coors.size() - 1);
  rectype newnum = 0;
  rectype arr[center_size * center_size];
  for (auto &i: center_flip_coors) {
    arr[i] = 1 && (num & cursor);
    cursor >>= 1;
  }
  for (auto &i: center_coors) {
    num <<= 1;
    num |= arr[i];
  }
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
}


