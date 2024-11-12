#include "leveldb/global.h"
// because nvm_node_size() is very slow, we use global variables to record KV size and change nvm_node
// just used to test more KVs
// in beta version, we support 2 nvm numa nodes
int nvm_node = 0;
int nvm_next_node = 0;
size_t nvm_free_space = 16L * 1024 * 1024 * 1024;
 bool nvm_node_has_changed = false;
size_t nvm_use = 0L;
long long nvm_total = 0L;
long long ops_num = 0L;


void NvmNodeSizeInit(const Options& options_) {
    nvm_node = options_.nvm_node;
    nvm_next_node = options_.nvm_next_node;
    long tmp;
    numa_node_size(nvm_node, &tmp);
    nvm_free_space = (size_t)tmp - 16L * 1024 * 1024 * 1024; 
	
}

void nvmn_size_delete(size_t s) {
    nvm_use-=s;
}

void NvmNodeSizeRecord(size_t s) {
    nvm_use += s;
    if (nvm_node_has_changed ) {
        return;
    }
    size_t tmp = (s + 4095) / 4096 * 4096; 
    if (tmp > nvm_free_space) {
        nvm_node_has_changed = true;
    } else {
        nvm_free_space -= tmp;
    }
}


void NvmUsagePrint() {
    std::cout << "Final Use NVM: " << 1.0 * nvm_use / 1024 / 1024 / 1024 << " GB" << std::endl;
    //std::cout << "Remainder of NVM space : " << 1.0 * nvm_free_space / 1024 / 1024 / 1024 << " GB" << std::endl;
}
