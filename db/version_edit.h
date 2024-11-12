// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_VERSION_EDIT_H_
#define STORAGE_LEVELDB_DB_VERSION_EDIT_H_

#include <set>
#include <utility>
#include <vector>
#include <unordered_map>
#include <util/persist.h>
#include "dbformat.h"

#include "db/dbformat.h"
#include "leveldb/datatable.h"

namespace leveldb {

class VersionSet;
class VersionSet_sst;

struct FileMetaData {
  FileMetaData() : refs(0), allowed_seeks(1 << 30), file_size(0) {}

  int refs;
  uint64_t number;
  uint32_t allowed_seeks;          // Seeks allowed until compaction
  uint64_t file_size;         // File size in bytes
  uint64_t total;             // Total count of keys
  uint64_t alive;             // Count of live keys
  InternalKey smallest;       // Smallest internal key served by table
  InternalKey largest;        // Largest internal key served by table

  PMtable* dt;
  bool mustquery;
};

class VersionEdit {
 public:
  VersionEdit() { Clear(); }
  ~VersionEdit() = default;

  void Clear();

  void SetComparatorName(const Slice& name) {
    has_comparator_ = true;
    comparator_ = name.ToString();
  }
  void SetLogNumber(uint64_t num) {
    has_log_number_ = true;
    log_number_ = num;
  }
  void SetPrevLogNumber(uint64_t num) {
    has_prev_log_number_ = true;
    prev_log_number_ = num;
  }
  void SetNextFile(uint64_t num) {
    has_next_file_number_ = true;
    next_file_number_ = num;
  }
  void SetLastSequence(SequenceNumber seq) {
    has_last_sequence_ = true;
    last_sequence_ = seq;
  }
  void SetCompactPointer(int level, const InternalKey& key) {
    compact_pointers_.push_back(std::make_pair(level, key));
  }

  // Add the specified file at the specified number.
  // REQUIRES: This version has not been saved (see VersionSet::SaveTo)
  // REQUIRES: "smallest" and "largest" are smallest and largest keys in file
  // ADD -------------------------------------
    void AddFile(int level, uint64_t file, uint64_t file_size,
               const InternalKey& smallest, const InternalKey& largest, PMtable* addtable) {
    FileMetaData f;
    f.number = file;
    f.file_size = file_size;
    f.smallest = smallest;
    f.largest = largest;
    f.dt = addtable;
    f.mustquery = false;
    new_files_.push_back(std::make_pair(level, f));
  }


  
  void AddFile(uint64_t file,
               uint64_t file_size,
               uint64_t total,
               uint64_t alive,
               const InternalKey& smallest,
               const InternalKey& largest) {
    FileMetaData f;
    f.number = file;
    f.file_size = file_size;
    f.total = total;
    f.alive = alive;
    f.smallest = smallest;
    f.largest = largest;
    _new_files_.push_back(f);
  }


// -------------------------------------

  void AddFile(int level, uint64_t file, uint64_t file_size,
               const InternalKey& smallest, const InternalKey& largest) {
    FileMetaData f;
    f.number = file;
    f.file_size = file_size;
    f.smallest = smallest;
    f.largest = largest;
    new_files_.push_back(std::make_pair(level, f));
  }


 void RemoveFile(int level, PMtable* file) {
    deleted_files_.insert(std::make_pair(level, file));
  }
  
  void DeleteFile(int level, uint64_t file) {
    deleted_files_sst.insert(std::make_pair(level, file));
  }
  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(const Slice& src);

  std::string DebugString() const;



 private:
  friend class VersionSet;
  friend class VersionSet_sst;

  typedef std::set<std::pair<int, PMtable*>> DeletedFileSet;
  typedef std::set<std::pair<int, uint64_t>> DeletedFileSet_SST;

  std::string comparator_;
  uint64_t log_number_;
  uint64_t prev_log_number_;
  uint64_t next_file_number_;
  SequenceNumber last_sequence_;
  bool has_comparator_;
  bool has_log_number_;
  bool has_prev_log_number_;
  bool has_next_file_number_;
  bool has_last_sequence_;


  std::vector<std::pair<int, InternalKey>> compact_pointers_;
  DeletedFileSet deleted_files_;
  DeletedFileSet_SST deleted_files_sst;
  std::vector<std::pair<int, FileMetaData>> new_files_;
  std::vector<FileMetaData> _new_files_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_VERSION_EDIT_H_
