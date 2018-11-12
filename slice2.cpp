#include "global.hpp"

void getptnsfromfile() {
  origin_ptn.resize(origin_ptn_size / sizeof(rectype));
  origin_file_stream.seekg(0);
  origin_file_stream.read((char*)origin_ptn.data(), origin_ptn_size);
  std::sort(origin_ptn.begin(), origin_ptn.end());
#ifdef DEBUG
  std::cout << "All patterns from file: " <<
    origin_file << std::endl << json(origin_ptn) <<
    std::endl;
#endif

}

void getpartialptnsfromfile(int pos, int size) {
  get_ptn.resize(size);
  origin_file_stream.seekg(pos*sizeof(rectype));
  origin_file_stream.read(
      (char*)get_ptn.data(), size*sizeof(rectype));
#ifdef DEBUG
  std::cout << "Get partial patterns from file: " << std::endl;
  std::cout << "Position: " << pos << ", Size: " << size <<
    std::endl << "Patterns: " << json(get_ptn) << std::endl;
#endif
}

void saveptnstofile() {
#ifdef NOSAVE
  std::cout << "saving " << target_ptn_size << " patterns to " << target_file << std::endl;
#else
  std::ofstream file(target_file, std::ios::binary|std::ios::out);
  rectype size = target_ptn.size() * sizeof(rectype);
  std::cout << "saving " << target_ptn.size() << " patterns to " << target_file << std::endl;
  std::sort(target_ptn.begin(), target_ptn.end());
  file.write((char*)target_ptn.data(), size);
#endif
}

void setTargetArrayToNumByCoor(
    rectype num, std::vector<int> &coor) {
  rectype cursor = 01ull << coor.size();
  int *arr = target_arr.data();
  for (auto &idx: coor) {
    cursor >>= 1;
    arr[idx] = 1 && (num & cursor);
  }
}

void setOriginArrayToNumByCoor(
    rectype num, std::vector<int> &coor) {
  rectype cursor = 01ull << coor.size();
  int *arr = origin_arr.data();
  for (auto &idx: coor) {
    cursor >>= 1;
    arr[idx] = 1 && (num & cursor);
  }
}

rectype getNumFromTargetArrayByCoor(
    std::vector<int> &coor) {
  rectype num = 0;
  int *arr = target_arr.data();
  for (auto &idx: coor) {
    num <<= 1;
    num |= arr[idx];
  }
  return num;
}

rectype getNumFromOriginArrayByCoor(
    std::vector<int> &coor) {
  rectype num = 0;
  int *arr = origin_arr.data();
  for (auto &idx: coor) {
    num <<= 1;
    num |= arr[idx];
  }
  return num;
}

void showOriginArrays() {
  int size = origin_size * origin_size;
  for (int i=0; i<size; i++) {
    if (i%origin_size == 0 && i>0) {
      std::cout.put('\n');
    }
    std::cout.put(origin_arr[i]?'1':' ');
    std::cout.put('-');
  }
  std::cout << std::endl;
}

void showTargetArrays() {
  int size = target_size * target_size;
  for (int i=0; i<size; i++) {
    if (i%target_size == 0 && i>0) {
      std::cout.put('\n');
    }
    std::cout.put(target_arr[i]?'0'+target_arr[i]:' ');
    std::cout.put('-');
  }
  std::cout << std::endl << std::endl;
}

rectype getOriginBottom(rectype num) {
  return num & origin_btm_mask;
}

rectype getTargetBottom(rectype num) {
  return num & target_btm_mask;
}

rectype getOriginTop(rectype num) {
  return num >> (origin_size - 1);
}

rectype getTargetTop(rectype num) {
  return num >> (target_size - 1);
}
