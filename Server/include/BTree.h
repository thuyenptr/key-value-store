//
// Created by lap11852 on 21/09/2018.
//
#pragma once
#ifndef KEYVALUESTORE_BTREE_H
#define KEYVALUESTORE_BTREE_H

#include "BTreeNode.h"
#include "utils.h"

class BTree {
private:
    BTreeNode* root;
public:
    char fname[20];
    int degree;
    int next_pos; // khi node moi tao thanh thi day la vi tri tiep theo
    int root_pos; // vi tri cua root
    FILE* f_bnode;
    BTree(char* fname);
    BTree();
    ~BTree();
    bool init();
    void insert(KEY key, VALUE value);
    VALUE search(KEY key);
    bool remove(KEY key);
    void traverse();
    void loaddata();
    void showPos();
    friend class BTreeNode;
};


#endif //KEYVALUESTORE_BTREE_H
