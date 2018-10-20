//
// Created by lap11852 on 17/09/2018.
//

#ifndef KEYVALUESTORE_STORAGE_H
#define KEYVALUESTORE_STORAGE_H
#include "KeyValueStore.h"


class BTreeStorage : public KeyValueStore {
private:
    BTree* tree;
public:
    BTreeStorage();
    BTreeStorage(char* fname, int mode);
    ~BTreeStorage();
    void showpos();
    void write_tree();
    bool read_tree();
    void traverse();
    VALUE get(KEY);
    void set(KEY, VALUE);
    bool exist(KEY);
    bool remove(KEY);
};

BTree* tree_init(char *fname);

#endif //KEYVALUESTORE_STORAGE_H
