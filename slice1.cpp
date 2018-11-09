#include "global.hpp"


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
    if (x >= y) {
      origin_coors.push_back(x*origin_size+y);
    }
  }

  for (int x=0; x<center_size; x++)
  for (int y=0; y<center_size; y++) {
    if (x >= y) {
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
    if (x >= y) {
      if (y!=0 and x!=target_size-1) {
        center_trangle_coors.push_back(x*target_size+y);
        bottom_origin_in_target_coors.push_back(x*target_size+y);
        left_origin_in_target_coors.push_back(x*target_size+y);
      } else if (y!=0) {
        bottom_origin_in_target_coors.push_back(x*target_size+y);
      } else if (x!=target_size-1) {
        left_origin_in_target_coors.push_back(x*target_size+y);
      }
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
}

void reset_arr() {
  std::fill(origin_arr.begin(), origin_arr.end(), 0);
  std::fill(target_arr.begin(), target_arr.end(), 0);
}
