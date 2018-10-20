//
// Created by lap11852 on 21/09/2018.
//

#include "../include/BTree.h"


BTree::BTree(char* fname)  {
    this->root = nullptr;
    this->f_bnode = fopen(fname, "r+");
    strcpy(this->fname, fname);
    this->root_pos = 0;
    this->next_pos = 0;
    this->degree = t;
}

BTree::BTree() {
    this->root = nullptr;
    this->root_pos = 0;
    this->next_pos = 0;
}

BTree::~BTree() {
    if (this->f_bnode) {
        fclose(this->f_bnode);
    }
}

bool BTree::init() {
    this->f_bnode = fopen(this->fname, "r+");
    if (!this->f_bnode) {
        return false;
//        this->f_bnode = fopen(this->fname, "w+");
//        fclose(this->f_bnode);
//        this->f_bnode = fopen(this->fname, "r+");
    }
    this->root = new BTreeNode();
    read_file(this, this->root, this->root_pos);
    return true;
}


void BTree::showPos() {
    cout << "next pos: " << this->next_pos << endl;
    cout << "root pos: " << this->root_pos << endl;
}

VALUE BTree::search(KEY key) {
    if (this->root) {
        return this->root->search(key);
    }
    return const_cast<char *>("");
}

void BTree::insert(KEY key, VALUE value) {
    DataStore newdata(key, value);
    if (!(this->next_pos)) { //this->root == nullptr
        if (this->root != nullptr) { // truong hop tree->init
            //delete(this->root);
            this->root = nullptr;
        }

        this->root_pos = this->next_pos;
        this->root = new BTreeNode();
        node_init(this->root, true, this);
        this->root->data[0] = newdata;
        this->root->num++;
        // write down to file
        write_file(this, this->root, this->root->pos);

    } else {
        // Read root from file, xu ly truong hop root doc roi thi sao??
        //root = new BTreeNode();
        //read_file(this, root, this->root_pos);
        if (this->root->num == 2*this->degree - 1) {
            int index = this->root->findKey(key);
            if (this->root->data[index] == newdata) {
                this->root->data[index] = newdata;
            } else {
                BTreeNode *s = new BTreeNode();
                node_init(s, false, this);
                // s is new root
                s->child[0] = this->root;
                s->child_pos[0] = this->root_pos;

                s->splitChild(0, root);

                int i = 0;
                if (s->data[i] < newdata) {
                    ++i;
                }

                // need child i => read from file
                // Read file
                //BTreeNode *c = new BTreeNode();
                //read_file(this, c, s->child_pos[i]);

                s->child[i]->insertNonFull(key, value);
                this->root = s;
                this->root_pos = s->pos;
            }

            write_file(this, this->root, this->root->pos);

        } else {
            int index = this->root->findKey(key);
            if (this->root->data[index] == newdata) {
                this->root->data[index] = newdata;
                write_file(this, this->root, this->root->pos);
                return;
            } else {
                root->insertNonFull(key, value);
            }
        }
    }
}

bool BTree::remove(KEY key) {
    if (!root) {
        return false;
    }
    bool res = root->remove(key);

    if (root->num == 0) {
        BTreeNode *tmp = root;
        if (root->leaf) {
            this->root_pos = 0;
            this->next_pos = 0;
            delete root;
            root = nullptr;
        } else {
            this->root = this->root->child[0];
            this->root_pos = this->root->pos;
            delete (tmp);
        }
    }
    return res;
}

void BTree::traverse() {
    if (this->root_pos != -1 && this->root) {
        this->root->traverse();
    }
}

void BTree::loaddata() {
    if (this->root_pos != -1) {
        this->root->loaddata();
    } else {
        cout << "Load data fail." << endl;
    }
}
