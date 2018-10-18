//
// Created by lap11852 on 09/10/2018.
//

#include "../include/BTree.h"

void write_file(BTree* tree, BTreeNode* node, int pos)
{
    if (node != nullptr && pos != -1) {
        fseek(tree->f_bnode, pos * sizeof(BTreeNode), 0);
        fwrite(node, sizeof(BTreeNode), 1, tree->f_bnode);
        fflush(tree->f_bnode);
    }
}

void read_file(BTree* tree, BTreeNode* node, int pos)
{
    if (pos != -1) {
        fseek(tree->f_bnode, pos * sizeof(BTreeNode), 0);
        fread(node, sizeof(BTreeNode), 1, tree->f_bnode);
        node->tree = tree;
        for (int i = 0; i < tree->degree; ++i) {
            node->child[i] = nullptr;
        }
    }
}