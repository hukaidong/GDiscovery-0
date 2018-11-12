#include "global.hpp"

#include <iomanip>


void init_target_size(int t_size) {
  std::ostringstream s_tfile, s_ofile;

  target_size = t_size;
  center_size = t_size - 2;
  origin_size = t_size - 1;

  s_ofile << "ptn" << origin_size << ".bin";
  s_tfile << "ptn" << target_size << ".bin";
  origin_file = s_ofile.str();
  target_file = s_tfile.str();

  origin_arr.resize(origin_size*origin_size);
  target_arr.resize(target_size*target_size);

  origin_file_stream.open(
      origin_file, std::ios::in|std::ios::binary|std::ios::ate);
  origin_ptn_size = origin_file_stream.tellg();
}

void init_coors() {
  origin_left.clear();
  origin_coors.clear();
  target_coors.clear();
  center_flip_coors.clear();
  center_trangle_coors.clear();
  bottom_origin_in_target_coors.clear();
  bottom_origin_in_target_coors_extra.clear();
  left_origin_in_target_coors.clear();
  left_origin_in_target_coors_extra.clear();
  for (int i=0; i<origin_size; i++)
    origin_left.push_back(origin_size*i);

  for (int x=0; x<origin_size; x++)
  for (int y=0; y<origin_size; y++) {
    if (x > y) {
      origin_coors.push_back(x*origin_size+y);
    }
  }

  for (int x=0; x<center_size; x++)
  for (int y=0; y<center_size; y++) {
    if (x > y) {
      center_coors.push_back(x*center_size+y);
      center_flip_coors.push_back((center_size-y)*center_size-x-1);
    }
  }
#ifdef DEBUG
  std::cout << "center_coors: " << json(center_coors) << std::endl;
  std::cout << "center_flip_coors: " << json(center_flip_coors) << std::endl;
#endif

  for (int x=0; x<target_size; x++)
  for (int y=0; y<target_size; y++) {
    if (x > y) {
      if (y!=0 and x!=target_size-1) {
        center_trangle_coors.push_back(x*target_size+y);
        bottom_origin_in_target_coors.push_back(x*target_size+y);
        left_origin_in_target_coors.push_back(x*target_size+y);
      } else if (y!=0) {
        bottom_origin_in_target_coors.push_back(x*target_size+y);
      } else if (x!=target_size-1) {
        left_origin_in_target_coors.push_back(x*target_size+y);
      }
      target_coors.push_back(x*target_size+y);
    }
  }

  std::set_difference(
      bottom_origin_in_target_coors.begin(), bottom_origin_in_target_coors.end(),
      center_trangle_coors.begin(), center_trangle_coors.end(),
      std::inserter(bottom_origin_in_target_coors_extra,
        bottom_origin_in_target_coors_extra.begin()));

#ifdef DEBUG
  std::cout << "bottom_origin_in_target_coors_extra: " <<
    json(bottom_origin_in_target_coors_extra) << std::endl;
#endif

  std::set_difference(
      left_origin_in_target_coors.begin(), left_origin_in_target_coors.end(),
      center_trangle_coors.begin(), center_trangle_coors.end(),
      std::inserter(left_origin_in_target_coors_extra,
        left_origin_in_target_coors_extra.begin()));
  left_origin_in_target_coors_rextra.resize(
      left_origin_in_target_coors_extra.size());
  std::reverse_copy(left_origin_in_target_coors_extra.begin(),
      left_origin_in_target_coors_extra.end(),
      left_origin_in_target_coors_rextra.begin());

#ifdef DEBUG
  std::cout << "left_origin_in_target_coors_extra: " <<
    json(left_origin_in_target_coors_extra) << std::endl;
#endif

  origin_btm_mask = (01ull << origin_size) - 1;
  target_btm_mask = (01ull << target_size) - 1;
  map_pos_offset = 16;
  map_offset_mask = (01ull << map_pos_offset) - 1;
  lb_coner_idx = target_size * (target_size-1);
}

void reset_arr() {
  std::fill(origin_arr.begin(), origin_arr.end(), 0);
  for (int i=0; i<origin_size; i++) {
    origin_arr[i*origin_size+i] = 1;
  }
  std::fill(target_arr.begin(), target_arr.end(), 0);
  for (int i=0; i<target_size; i++) {
    target_arr[i*target_size+i] = 1;
  }
}

void verify_avail_target_ptn() {
  int cursor = lb_coner_idx;
  rectype result;
  while (target_arr[cursor-target_size] == 0) cursor -= target_size;
  while (target_arr[cursor+1] == 0) cursor += 1;
  int x = cursor % target_size;
  int y = cursor / target_size;
  /* Case 0: */
  do {
#ifdef DEBUG
    std::cout << "========================" << std::endl;
    std::cout << "x: " << x << ", y: " << y << std::endl;
    showTargetArrays();
#endif
    if (x == y - 1) {
      target_arr[lb_coner_idx] = 0;
      bool iszero = 0;
#ifdef DEBUG
        std::cout << "following cooridinations are scaned" << std::endl;
#endif
      for (int xi = 0; xi <= x; xi++)
      for (int yi = y; yi < target_size; yi++) {
#ifdef DEBUG
        std::cout <<
          std::setw(6) << xi <<
          std::setw(6) << yi <<
          std::setw(6) << target_arr[yi*target_size + xi] <<
          std::endl;
#endif
        iszero |= target_arr[yi*target_size+xi];
      }
      if (iszero == 0) {
        result = getNumFromTargetArrayByCoor(target_coors);
        target_ptn.push_back(result);
#ifdef DEBUG
        std::cout << "Hit on coner = 0" << std::endl;
#endif
      }
    }
  } while (0);
  /* Case 1: */
  do {
#ifdef DEBUG
    std::cout << "value_sqx: " << target_arr[lb_coner_idx+x+1] << std::endl;
    std::cout << "value_sqy: " << target_arr[(y-1)*target_size] << std::endl;
#endif
    if(target_arr[lb_coner_idx+x+1] && target_arr[(y-1)*target_size]) {
      target_arr[lb_coner_idx] = 1;
      result = getNumFromTargetArrayByCoor(target_coors);
      target_ptn.push_back(result);
#ifdef DEBUG
      std::cout << "Hit on coner = 1" << std::endl;
#endif
    }
  } while (0);
}
