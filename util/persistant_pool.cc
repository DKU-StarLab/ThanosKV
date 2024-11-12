
#include "leveldb/persistant_pool.h"
#include <mutex>
#include <libpmemobj.h>
#include <libpmem.h>
#include <stdexcept>

#include <cstdio>
#include <leveldb/nvm_option.h>

namespace leveldb {
namespace nvram {

#define LAYOUT_NAME "PMINDEXDB"

static PMEMobjpool* pm_pool;
// static bool init = true;
static uint64_t allocs = 0;

char* pmemaddr=nullptr;

int is_pmem;
char* raw_;
static std::mutex pmem_mutex;

int nvm_file_exists(const char* file) { return access(file, F_OK); }

void create_pool(const std::string& dir, const size_t& size) {
    printf("Attempting to open NVM pool\n");
    pm_pool = pmemobj_open(dir.data(), LAYOUT_NAME);
    pmemaddr = (char*)pmemobj_direct(pm_pool);
    if (pm_pool == nullptr) {
    
        pm_pool = pmemobj_create(dir.data(), LAYOUT_NAME, size, 0666);
        
        if (pm_pool == nullptr) {
            fprintf(stderr, "pmem create error\n");
            perror(dir.data());
            exit(1);
        }
    } else {
        printf("NVM pool opened successfully\n");
    }
}

void close_pool() {
   // fprintf(stdout, "pmem allocs %lu\\n", allocs);
    pmemobj_close(pm_pool);
}

void pfree(void* ptr) {
    PMEMoid oid = pmemobj_oid(ptr); 
    if (OID_IS_NULL(oid)) {
        return; // Don't attempt to free a null pointer.
    } 
    pmemobj_free(&oid);
}

void* pmalloc(size_t size) {
    std::lock_guard<std::mutex> lock(pmem_mutex);
    void* ptr;
    allocs++;
    PMEMoid oid; 
    
    if (pmemobj_zalloc(pm_pool, &oid, size, 0) != 0) 
        return nullptr; 

    return pmemobj_direct(oid);
}


}
}