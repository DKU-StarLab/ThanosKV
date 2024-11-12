#include "leveldb/ff_btree.h"
#include "leveldb/ff_btree_iterator.h"

namespace leveldb {

/*
 *  class btree
 */
FFBtree::FFBtree(){
  root = new Page();
  height = 1;
}

void FFBtree::setNewRoot(void* new_root) {
  this->root = new_root;
  clflush((char*)&(this->root),sizeof(void*));
  ++height;
}

void* FFBtree::Search(const entry_key_t& key){
  Page* p = (Page*)root;

  while(p->hdr.leftmost_ptr != NULL) {
    p = (Page *)p->linear_search(key);
  }

  Page *t;
  while((t = (Page *)p->linear_search(key)) == p->hdr.sibling_ptr) {
    p = t;
    if(!p) {
      break;
    }
  }
  return (char *)t;
}

// void* FFBtree::Insert(const entry_key_t& key, void* right){ //need to be string
//   Page* p = (Page*)root;

//   while(p->hdr.leftmost_ptr != NULL) {
//     p = (Page*)p->linear_search(key);
//   }

//   void* ret = nullptr;
//   if(!p->store(this, NULL, key, right, true, nullptr, &ret)) { // store
//     return Insert(key, right);
//   }
//   return ret;
// }


void* FFBtree::Insert(const entry_key_t& key, void* right) {
  while (true) {
    Page* p = (Page*)root;

    while (p->hdr.leftmost_ptr != NULL) {
      p = (Page*)p->linear_search(key);
    }

    void* ret = nullptr;
    if (p->store(this, NULL, key, right, true, nullptr, &ret)) { // store successfully
      return ret;
    }
    // otherwise, loop back and try again
  }
}

// void* FFBtree::Insert(const entry_key_t& key, void* right) {
//   const int maxRetries = 10; // 我们为重试设置一个最大值
//   int currentTry = 0;

//   while (currentTry < maxRetries) {
//     Page* p = (Page*)root;

//     while (p->hdr.leftmost_ptr != NULL) {
//       p = (Page*)p->linear_search(key);
//     }

//     void* ret = nullptr;
//     if (p->store(this, NULL, key, right, true, nullptr, &ret)) { 
//       return ret; // 如果存储成功，直接返回
//     }

//     // 如果我们到这里，表示存储失败了。我们稍等片刻再重试。
//     currentTry++;
//     std::this_thread::sleep_for(std::chrono::milliseconds(10 * currentTry)); // 逐渐增加等待时间，给其他线程操作的机会
//   }

//   // 如果我们已经尝试了太多次，就抛出一个异常或返回错误。
//   throw std::runtime_error("Unable to insert after multiple attempts.");
// }


void* FFBtree::InsertInternal(void* left, const entry_key_t& key,
                             void* right, uint32_t level) {
  if(level > ((Page *)root)->hdr.level)
    return nullptr;

  Page *p = (Page *)this->root;

  while(p->hdr.level > level)
    p = (Page *)p->linear_search(key);

  void* ret = nullptr;
  if(!p->store(this, NULL, key, right, true, nullptr, &ret)) {
    return InsertInternal(left, key, right, level);
  }
  return ret;
}

void FFBtree::Remove(const entry_key_t& key) {
  Page* p = (Page*)root;

  while(p->hdr.leftmost_ptr != NULL){
    p = (Page*) p->linear_search(key);
  }

  Page *t;
  while((t = (Page *)p->linear_search(key)) == p->hdr.sibling_ptr) {
    p = t;
    if(!p)
      break;
  }

  if(p) {
    if(!p->remove(this, key)) {
      Remove(key);
    }
  }
  else {
    // printf("not found the key to delete %lu\n", key);
  }
}

void FFBtree::RemoveInternal(const entry_key_t& key, void* ptr, uint32_t level,
                             entry_key_t* deleted_key, bool* is_leftmost_node,
                             Page** left_sibling) {
  if(level > ((Page *)this->root)->hdr.level)
  return;

  Page *p = (Page *)this->root;

  while(p->hdr.level > level) {
    p = (Page *)p->linear_search(key);
  }

  if((char *)p->hdr.leftmost_ptr == ptr) {
    *is_leftmost_node = true;
    return;
  }

  *is_leftmost_node = false;

  for(int i=0; p->records[i].ptr != NULL; ++i) {
    if(p->records[i].ptr == ptr) {
      if(i == 0) {
        if((char *)p->hdr.leftmost_ptr != p->records[i].ptr) {
          *deleted_key = p->records[i].key;
          *left_sibling = p->hdr.leftmost_ptr;
          p->remove(this, *deleted_key, false, false);
          break;
        }
      }
      else {
        if(p->records[i - 1].ptr != p->records[i].ptr) {
          *deleted_key = p->records[i].key;
          *left_sibling = (Page *)p->records[i - 1].ptr;
          p->remove(this, *deleted_key, false, false);
          break;
        }
      }
    }
  }
}

FFBtreeIterator* FFBtree::GetIterator() {
  return new FFBtreeIterator(this);
}

}
