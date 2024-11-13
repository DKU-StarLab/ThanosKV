#pragma once

#include <string>

namespace leveldb {

struct NVMOption {
  //SSD path
  std::string sst_path="..";
};

}  // namespace leveldb