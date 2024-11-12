
1. status = olddt->Compact(newdt,smallest_snapshot);

2. Status PMtable::Compact(PMtable* dtable, SequenceNumber snum) {
    ..
table_.Compact(&(dtable->table_), snum);
..
		return Status::OK();
}
3. template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Compact(SkipList<Key, Comparator>* list, SequenceNumber snum) {

  Node *x = list->head_->Next(0);
  Node *y, *ypre[kMaxHeight], *xpre[kMaxHeight];
  for (int i = 0; i < kMaxHeight; i++) {
    if (i < list->GetMaxHeight()) {
      xpre[i] = list->head_;
    } else {
      xpre[i] = nullptr;
    }
  }

  bool first = true;
  while (x != nullptr) {
    insertingnode.store(x, std::memory_order_release);
    DeleteNode(xpre, x);
    Insert(x, ypre);
	  
    y = x;
    PreNext(ypre, y->height);

    // Set smallest
    if (first) {
      if (smallest == nullptr || NewCompare(y, smallest, false, 0) == 0b01) {
        smallest = y;
      }
      first = false;
    }
// 0b0010 : 이진수로 표현된 2
// 0b10 : 이진수로 표현된 2
// 0b11 : 이진수로 표현된 3
    // LargeTable duplication
    while (y->Next(0) != nullptr) {
      int r = NewCompare(y, y->Next(0), true, snum);
      if (r == 0b0010) {
        for (int i = 0; i < GetMaxHeight(); i++) {
          if (largest[i] = y->Next(0)) {
            largest[i] = ypre[i];
          } else {
            break;
          }
        }
        DeleteNode(ypre, y->Next(0));
      } else if ((r & 0b11) == 0b10) {
        y = y->Next(0);
        PreNext(ypre, y->height);
      } else {
        break;
      }
    }

    // Jump obsolescent node in small table
    x = xpre[0]->Next(0);
    if (x == nullptr) {

    } else {
      bool flag = true;
      int r;
      do {
        if (flag) {
          r = NewCompare(insertingnode.load(std::memory_order_relaxed), x, true, snum);
          flag = false;
        } else {
          r = NewCompare(xpre[0], x, true, snum);
        }
        if (r == 0b0010) {
          PreNext(xpre, x->height);
          x = x->Next(0);
        } else {
          break;
        }
      } while (x != nullptr);
    }
  }
  insertingnode.store(nullptr, std::memory_order_release);

  // Set Largest
  for (int i = 0; i < GetMaxHeight(); i++) {
    if (NewCompare(ypre[i], largest[i], false, 0) == 0b11) {
      largest[i] = ypre[i];
    }
  }
  
  arena_->ReceiveArena(list->arena_);
  list->arena_->SetTransfer();
  return true;
}