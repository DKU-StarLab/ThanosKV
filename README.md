## ThanosKV: A Holistic Approach to Utilize NVM for LSM-tree based Key-Value Stores
ThanosKV is a novel LSM-tree-based key-value store that holistically exploits the non-volatility and byte-addressability of NVM.

### Dependencies
> If you do not have access to an NVM device, please refer to the following website.
- https://pmem.io/blog/2016/02/how-to-emulate-persistent-memory


### Download
```bash
git clone --recurse-submodules https://github.com/ErosBryant/Thanoskv.git

```

## Building

```bash
cd Thanoskv && mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
```

### Set nvm version

```
scripts/mount_dax.sh
```



