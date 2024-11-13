## Function description
- WriteLevel0Table: Memtable to Pmtable
- WriteLeveltoSsTable: Pmtable to SSTable
- DoCompactionWork: Compaction of Pmtable
- DoCompactionWork_sst: Compaction of SSTable
- dbname_(dbname): NVM path
- dbname_ssd_(raw_options.nvm_option.sst_path): SSD path
- leveldb::nvram::create_pool: create_pool nvm pool
- CreateBtreeIndex: creat nvm index
- VersionSet_sst: version control of SSTable
- VersionSet:version control of Pmtable
  
