
#ifndef STORAGE_LEVELDB_DB_PMtable_H_
#define STORAGE_LEVELDB_DB_PMtable_H_

#include <string>

#include "db/dbformat.h"
#include "db/skiplist.h"
#include "leveldb/db.h"
#include "leveldb/status.h"
#include "util/arena.h"
#include "db/memtable.h"
#include "leveldb/options.h"
#include "leveldb/mergeablebloom.h"

namespace leveldb {

class InternalKeyComparator;
class PMtableIterator;
//typedef SkipList<char*, KeyComparator> dTable;

class PMtable {
 public:
 //初始化时，需要传入一个memtable，用于初始化skiplist

  explicit PMtable(const InternalKeyComparator& comparator,MemTable* mem, const Options& options_);
  explicit PMtable(const InternalKeyComparator& comparator,const string dbname_, MemTable* mem, const Options& options_);
  explicit PMtable(const InternalKeyComparator& comparator);

  PMtable(const PMtable&) = delete;
  PMtable& operator=(const PMtable&) = delete;

  ~PMtable();	// When compaction is completed, should delete the old PMtable

  // Increase reference count.
  void Ref() { ++refs_; }

  // Drop reference count.  Delete if no more references exist.
  void Unref() {
    --refs_;
    assert(refs_ >= 0);
    if (refs_ <= 0) {
      delete this;
    }
  }

  // Returns an estimate of the number of bytes of data in use by this
  // data structure. 
  //Can be replaced by a number, when compation is completed, we can recalculate it
  //used to judge if the db should compact
  size_t ApproximateMemoryUsage();

  // Return an iterator that yields the contents of the PMtable.
  //
  // while the returned iterator is live.  The keys returned by this
  // iterator are internal keys encoded by AppendInternalKey in the
  // db/format.{h,cc} module.
  Iterator* NewIterator();

  // If PMtable contains a value for key, store it in *value and return true.
  // If PMtable contains a deletion for key, store a NotFound() error
  // in *status and return true.
  // Else, return false.
  // Some get operation will start with the jumpflag node instead of the start of skiplist
  bool Get(const LookupKey& key, std::string* value, Status& s);

  Status Compact(PMtable* smalltable, SequenceNumber snum);

 private:

  friend class PMtableIterator;
  friend class PMtableBackwardIterator;

  KeyComparator comparator_;
  int refs_;

 public:
  Arena arena_;
  MergeableBloom* bloom_;
  nvm_Table table_;
  bool IsLastTable;
  string dbname;
};

}	// namespace leveldb

#endif //STORAGE_LEVELDB_DB_PMtable_H_
