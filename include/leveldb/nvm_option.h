#pragma once

#include <string>

namespace leveldb {

struct NVMOption {
  std::string pmem_path = "/mnt/pmemdir";
  std::string sst_path="/media/eros/new1";
  std::string sst_path2="/home/eros/workspace-lsm/Thanoskv/test";
  //size_t nvm_write_buffer_size = 6* 1024 * 1024 * 1024;
  // char buf[100];
  // snprintf(buf, sizeof(buf), "%s/_PM_sstable.pool",
  //          NVMOption.pmem_path.c_str());
  // std::string pol_path(buf, strlen(buf));


};

}  // namespace leveldb