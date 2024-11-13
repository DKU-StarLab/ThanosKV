## ThanosKV: A Holistic Approach to Utilize NVM for LSM-tree based Key-Value Stores
ThanosKV is a novel LSM-tree-based key-value store that holistically exploits the non-volatility and byte-addressability of NVM.

### Dependencies
> If you do not have an NVM device, please refer to the following website.
- https://pmem.io/blog/2016/02/how-to-emulate-persistent-memory


### Download
```bash
git clone --recurse-submodules https://github.com/DKU-StarLab/ThanosKV.git
```

## Building
> This project supports [CMake](https://cmake.org/) out of the box.

cmake_minimum_required(VERSION 3.9)

```bash
cd Thanoskv && mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
```

```
scripts/mount_dax.sh
```


<!-- ```

Set SSD path: /Thanoskv/include/leveldb/nvm_option.h
``` -->






