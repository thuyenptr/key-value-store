//
// Created by lap11852 on 17/09/2018.
//

#include "../include/BTreeStorage.h"

BTreeStorage::BTreeStorage() {

};

BTreeStorage::~BTreeStorage() {
    if (this->tree) {
        delete this->tree;
    }
}

BTreeStorage::BTreeStorage(char* fname, int mode) {
    if (mode == 0) {
        // Create new BTree
        this->tree = tree_init(fname);
    } else if (mode == 1) {
        // From file
        if (!this->read_tree()) {
            this->tree = tree_init(fname);
        }
    }
}

void BTreeStorage::write_tree() {
    FILE* f_tree = fopen("tree.dat", "w+");
    fwrite(this->tree, sizeof(BTree), 1, f_tree);
    fclose(f_tree);
}

bool BTreeStorage::read_tree() {
    FILE* f_tree = fopen("tree.dat", "r+");
    if (f_tree == nullptr) {
        f_tree = fopen("tree.dat", "w+");
        fclose(f_tree);
        f_tree = fopen("tree.dat", "r+");
    }
    this->tree = new BTree();
    fread(this->tree, sizeof(BTree), 1, f_tree);

    if (!this->tree->init()) {
        return false;
    }
    cout << "next pos: " << this->tree->next_pos << endl;
    cout << "root_pos: " << this->tree->root_pos << endl;
    // load time
    timespec stop, start;
    clock_gettime(CLOCK_REALTIME, &start);

    this->tree->loaddata();
    cout << "tree: " << endl;
    this->tree->traverse();
    clock_gettime(CLOCK_REALTIME, &stop);

    double accum = ((stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec)) / (BILLION * 1.0);
    cout << "Load time: " << accum << endl;
    fclose(f_tree);
    return true;
}

void BTreeStorage::showpos() {
    this->tree->showPos();
}

void BTreeStorage::traverse() {
    this->tree->traverse();
}


VALUE BTreeStorage::get(KEY key) {
    VALUE result = this->tree->search(key);
    return result;
}


void BTreeStorage::set(KEY key, VALUE value) {
    this->tree->insert(key, value);
}


bool BTreeStorage::exist(KEY key) {
    return true;
}


bool BTreeStorage::remove(KEY key) {
    return this->tree->remove(key);
}

BTree *tree_init(char *fname) {
    BTree *tree = new BTree(fname);
    strcpy(tree->fname, fname);
    tree->f_bnode = fopen(fname, "r+");
    if (!tree->f_bnode) {
        tree->f_bnode = fopen(fname, "w+");
        fclose(tree->f_bnode);
        tree->f_bnode = fopen(fname, "r+");

        // FILE* t_file = fopen("tree.dat", "w+");
        // fclose(t_file);
        // t_file = fopen("tree.dat", "r+");
    }
    tree->root_pos = 0;
    tree->next_pos = 0;
    tree->degree = t;
    return tree;
}