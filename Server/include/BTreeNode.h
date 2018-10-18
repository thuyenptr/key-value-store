//
// Created by lap11852 on 21/09/2018.
//

#ifndef KEYVALUESTORE_BTREENODE_H
#define KEYVALUESTORE_BTREENODE_H
#pragma once
#include "DataStore.h"

class BTree;

class BTreeNode {
public:
    int degree;
    BTreeNode* child[2*t];
    int num;
    bool leaf;

    BTree* tree;
    int pos; // vi tri de tinh offset
    int child_pos[2*t];

    DataStore data[2*t - 1];

    BTreeNode(bool leaf, BTree* tree);
    BTreeNode();
    ~BTreeNode();

    void traverse();

    void loaddata();

    VALUE search(KEY key);

    void insertNonFull(KEY key, VALUE value);
    void splitChild(int i, BTreeNode* y);

    int findKey(KEY key);
    // delete method
    bool remove(KEY key);

    bool removeFromLeaf(int index);

    bool removeFromNonLeaf(int index);

    DataStore getPredecessor(int index);

    DataStore getSuccessor(int index);

    void fill(int index);

    void borrowFromPrev(int index);

    void borrowFromNext(int index);

    void merge(int index);

    friend class BTree;
};

void node_init(BTreeNode *node, bool isLeaf, BTree *tree);

#endif //KEYVALUESTORE_BTREENODE_H
