
1. status = olddt->Compact(newdt,smallest_snapshot);

2. Status PMtable::Compact(PMtable* dtable, SequenceNumber snum) {
    ..
table_.Compact(&(dtable->table_), snum);
..
		return Status::OK();
}

3. template <typename Key, class Comparator>
SkipList<Key, Comparator>* MergeSkipLists(const SkipList<Key, Comparator>& skipList1, const SkipList<Key, Comparator>& skipList2) {

    SkipList<Key, Comparator>* mergedList = new SkipList<Key, Comparator>();


    typename SkipList<Key, Comparator>::Iterator it1(&skipList1);
    typename SkipList<Key, Comparator>::Iterator it2(&skipList2);

    it1.SeekToFirst();
    it2.SeekToFirst();


    while (it1.Valid() && it2.Valid()) {
        int cmp = Comparator()(it1.key(), it2.key());

        if (cmp < 0) {  
            mergedList->Insert(it1.key());
            it1.Next();
        } else if (cmp > 0) {
            mergedList->Insert(it2.key());
            it2.Next();
        } else {  
            if (!areNodesDuplicates(it1.node_, it2.node_)) {
            mergedList->Insert(it1.key());  
        }
          it1.Next();
          it2.Next();
        }
    }


    while (it1.Valid()) {
        mergedList->Insert(it1.key());
        it1.Next();
    }

    // 插入来自skipList2的剩余键（如果有）
    while (it2.Valid()) {
        mergedList->Insert(it2.key());
        it2.Next();
    }

    return mergedList;
}

template <typename Key, class Comparator>
bool areNodesDuplicates(const typename SkipList<Key, Comparator>::Node* x, const typename SkipList<Key, Comparator>::Node* y) {
    return x->key == y->key && x->len == y->len;
}
