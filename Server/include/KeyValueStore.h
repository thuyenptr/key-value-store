//
// Created by lap11852 on 17/09/2018.
//

#ifndef KEYVALUESTORE_KEYVALUESTORE_H
#define KEYVALUESTORE_KEYVALUESTORE_H
#include "BTree.h"

class KeyValueStore {
public:
    virtual VALUE get(KEY) = 0;
    virtual void set(KEY, VALUE) = 0;
    virtual bool exist(KEY) = 0;
    virtual bool remove(KEY) = 0;
    virtual void write_tree() = 0;
};

#endif //KEYVALUESTORE_KEYVALUESTORE_H
