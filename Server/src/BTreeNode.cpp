//
// Created by lap11852 on 21/09/2018.
//

#include "../include/BTreeNode.h"
#include "../include/BTree.h"

BTreeNode::BTreeNode(bool leaf, BTree* tree) {
    this->leaf = leaf;
    this->num = 0;
    //this->child = new BTreeNode*[2*t];
    this->tree = tree;
    this->degree = t;
    this->pos = tree->next_pos;
    this->tree->next_pos++;
    for (int i = 0; i < 2*t; ++i) {
        this->child_pos[i] = -1;
    }
    for(int i = 0; i < 2*t; i++) {
        this->child[i] = nullptr;
    }
}

void node_init(BTreeNode *node, bool isLeaf, BTree *tree) {

    node->leaf = isLeaf;
    node->num = 0;
    node->pos = tree->next_pos;
    tree->next_pos++;
    node->tree = tree;
    node->degree = t;

    for(int i = 0; i < 2*t; i++) {
        node->child_pos[i] = -1;
    }
    for(int i = 0; i < 2*t; i++) {
        node->child[i] = nullptr;
    }
}


BTreeNode::BTreeNode() {
    this->pos = -1;
    //this->child = new BTreeNode*[2*t];
    this->degree = t;
    this->num = 0;
    for (int i = 0; i < 2*t; ++i) {
        this->child_pos[i] = -1;
    }
    for(int i = 0; i < 2*t; i++) {
        this->child[i] = nullptr;
    }
}


BTreeNode::~BTreeNode() {
    if (this->leaf) {
        cout << "Destructor calling ..............................." << endl;
        for (int i = 0; i < 2 * this->degree; ++i) {
            if (this->child[i] != nullptr) {
                delete this->child[i];
                this->child[i] = nullptr;
            }
        }
    }
}

void BTreeNode::insertNonFull(KEY key, VALUE value) {
    DataStore newdata(key, value);
    int i = this->num - 1;

    if (this->leaf) {
        // find location
        int index = this->findKey(key);
        if (this->data[index] == newdata) { // Neu co key tren node
            this->data[index] = newdata;
            return;
        } else {
            // move greater keys to right
            while (i >= 0 && (this->data[i] > newdata)) {
                this->data[i + 1] = this->data[i];
                i--;
            }
            // Insert new key
            this->data[i + 1] = newdata;
            this->num++;
        }
        // ghi xuong disk => this
        write_file(this->tree, this, this->pos);
    }
    else {
        // find key
        int index = this->findKey(key);
        if (this->data[index] == newdata) {
            this->data[index] = newdata;
            write_file(this->tree, this, this->pos);
            return;
        } else {
            // find child
            while (i >= 0 && (this->data[i] > newdata)) {
                --i;
            }

            // can node con => xuong dia doc len child i + 1
            //BTreeNode* c = new BTreeNode();
            //read_file(this->tree, c, this->child_pos[i+1]);
            if (this->child[i+1]->num == 2*this->degree - 1) {
                // Kiem tra neu tren child nay da co key can chen => update no
                int index = this->child[i+1]->findKey(key);
                if (this->child[i+1]->data[index] == newdata) {
                    this->child[i+1]->data[index] = newdata;
                    return;
                } else {
                    // child full
                    this->splitChild(i+1, this->child[i+1]);
                    if (this->data[i+1] < newdata) {
                        ++i;
                    }
                }
            }

            // doc node con i+1 len
            //read_file(this->tree, c, this->child_pos[i+1]);
            //c->insertNonFull(key, value);

            this->child[i+1]->insertNonFull(key, value);
        }
    }
}

void BTreeNode::splitChild(int index, BTreeNode *y) {
    // i: chi so cua node medium
    // Node y full key 2*t - 1 key
    BTreeNode* z = new BTreeNode();
    node_init(z, y->leaf, y->tree);
    z->num = this->degree - 1;

    // copy the last t - 1 key from full node to z
    for (int i = 0; i < this->degree - 1; ++i) {
        z->data[i] = y->data[this->degree + i];
    }

    if (!y->leaf) {
        for (int i = 0; i < this->degree; ++i) {
            z->child[i] = y->child[this->degree + i];
            z->child_pos[i] = y->child_pos[this->degree + i];
        }
    }

    y->num = this->degree - 1;

    // new child to root
    // Tao o de bo node moi vao
    for (int i = this->num; i >= index + 1; --i) {
        this->child[i+1] = this->child[i];
        this->child_pos[i+1] = this->child_pos[i];
    }

    // link new child
    this->child[index+1] = z;
    this->child_pos[index+1] = z->pos;

    // Tao o de bo key vao
    for (int i = this->num - 1; i >= index; --i) {
        this->data[i+1] = this->data[i];
    }

    // copy middle key to this node
    this->data[index] = y->data[this->degree - 1];

    this->num++;

    // write all node change: this, y, z
    write_file(this->tree, this, this->pos);
    write_file(this->tree, y, y->pos);
    write_file(this->tree, z, z->pos);
}

void BTreeNode::loaddata() {
    if (this->pos == -1) {
        return;
    }
    int i = 0;
    for (; i < this->num; ++i) {
        if (!this->leaf) {
            //BTreeNode *node = (BTreeNode*) malloc(sizeof(BTreeNode));
            BTreeNode *node = new BTreeNode();
            read_file(this->tree, node, this->child_pos[i]);
            this->child[i] = node;
            node->loaddata();
        }
    }
    if (!this->leaf && this->num != 0) {
        //BTreeNode *node = (BTreeNode*) malloc(sizeof(BTreeNode));
        BTreeNode *node = new BTreeNode();
        read_file(this->tree, node, this->child_pos[i]);
        this->child[i] = node;
        node->loaddata();
    }
}

void BTreeNode::traverse() {
    cout << "begin node" << endl;
    int i = 0;
    for (; i < this->num; ++i) {
        if (!this->leaf) {
            this->child[i]->traverse();
        }
        cout << this->data[i] << endl;
    }
    cout << "end node" << endl;
    if (!this->leaf) {
        this->child[i]->traverse();
    }
}

VALUE BTreeNode::search(KEY key) {
    DataStore newdata(key, const_cast<char *>(""));
    int i = 0;
    while (i < this->num && this->data[i] < newdata)
        ++i;
    if (this->data[i] == newdata && i < this->num) { // tao da sua cho nay
        return this->data[i].value;
    }

    // Di het la ma chua thay
    if (this->leaf) {
        return const_cast<char *>("");
    }

    return this->child[i]->search(key);
}


int BTreeNode::findKey(KEY key) {
    DataStore newdata(key, const_cast<char *>(""));
    int index = 0;
    while (index < this->num && (this->data[index] < newdata)) {
        index++;
    }
    return index;
}

bool BTreeNode::remove(KEY key) {
    DataStore newdata(key, const_cast<char *>(""));
    bool res;
    int index = this->findKey(key);
    // Neu key nam trong node nay
    if (index < this->num && (this->data[index] == newdata)) {
        if (this->leaf) {
            res = this->removeFromLeaf(index);
        } else {
            res = this->removeFromNonLeaf(index);
        }
        // cap nhat node nay xuong file
        write_file(this->tree, this, this->pos);
    } else {
        if (this->leaf) {
            cout << "Key does not exist" << endl;
            return false;
        }

        bool flag = (index == this->num);

        // neu node nho hon t key
        // fill no
        if (this->child[index]->num < this->degree) {
            this->fill(index);
        }

        if (flag && index > this->num) {
            this->child[index - 1]->remove(key);
            if (this->child[index-1] != nullptr) {
                write_file(this->tree, this->child[index-1], this->child[index - 1]->pos);
            }
        } else {
            this->child[index]->remove(key);
        }
        if (this->child[index] != nullptr) {
            write_file(this->tree, this->child[index], this->child[index]->pos);
        }
        res = true;
    }
    return res;
}

// truong hop neu la la => remove
bool BTreeNode::removeFromLeaf(int index) {
    for (int i=index+1; i<this->num;++i) {
        this->data[i-1] = this->data[i];
    }
    this->num--;
    return true;
}

// khong phai la
bool BTreeNode::removeFromNonLeaf(int index) {
    char k[1024];
    strcpy(k,this->data[index].key);
    bool res;

    if (this->child[index]->num >= this->degree) {
        DataStore pred = this->getPredecessor(index); // tim phan tu the mang
        this->data[index] = pred;
        res = this->child[index]->remove(pred.key); // tiep tuc de quy xoa key
    } else if (this->child[index + 1]->num >= this->degree) { // tim phan tu the mang ben phai
        DataStore succ = this->getSuccessor(index);
        this->data[index] = succ;
        res = this->child[index+1]->remove(succ.key);
    } else { // truong hop ca hai child deu co so key = t - 1

        this->merge(index); // merge 2 child
        res = this->child[index]->remove(k);
    }
    if (this->child[index] != nullptr) {
        write_file(this->tree, this->child[index], this->child_pos[index]);
    }
    if (this->child[index + 1] != nullptr) {
        write_file(this->tree, this->child[index + 1], this->child_pos[index + 1]);
    }
    return res;
}

DataStore BTreeNode::getPredecessor(int index) {
    BTreeNode *cur = this->child[index];
    while (!cur->leaf) {
        cur = cur->child[cur->num];
    }
    int idx = cur->num-1;
    return cur->data[idx];
}

DataStore BTreeNode::getSuccessor(int index) {
    BTreeNode *cur = this->child[index + 1];
    while (!cur->leaf) {
        cur = cur->child[0];
    }
    return cur->data[0];
}

void BTreeNode::fill(int index) {
    if (index != 0 && this->child[index - 1]->num >= this->degree) {
        borrowFromPrev(index);
    } else if (index != this->num && this->child[index+1]->num >= this->degree) {
        borrowFromNext(index);
    } else {
        if (index != this->num) {
            this->merge(index);
        } else {
            this->merge(index - 1);
        }
    }
}

void BTreeNode::borrowFromPrev(int index) {
    BTreeNode* child = this->child[index];
    BTreeNode* sibling = this->child[index - 1];

    for (int i = child->num - 1; i >= 0; --i) {
        child->data[i+1] = child->data[i];
    }

    if (!child->leaf) {
        for (int i = child->num; i >= 0; --i) {
            child->child[i+1] = child->child[i];
            child->child_pos[i+1] = child->child_pos[i];
        }
    }


    child->data[0] = this->data[index-1];

    if (!child->leaf) {
        child->child[0] = sibling->child[sibling->num];
        //-------------------------------------
        //sibling->child[sibling->num] = nullptr;
        //-------------------------------------
        child->child_pos[0] = sibling->child_pos[sibling->num];
    }

    this->data[index-1] = sibling->data[sibling->num-1];

    child->num += 1;
    sibling->num -= 1;

    write_file(this->tree, this, this->pos);
    write_file(this->tree, child, child->pos);
    write_file(this->tree, sibling, sibling->pos);
}

void BTreeNode::borrowFromNext(int index) {
    BTreeNode* child = this->child[index];
    BTreeNode* sibling = this->child[index + 1];

    child->data[child->num] = this->data[index];

    if (!child->leaf) {
        child->child[(child->num) + 1] = sibling->child[0];
        //-------------------------------------------------
        //sibling->child[0] = nullptr;
        //-------------------------------------------------
        child->child_pos[(child->num) + 1] = sibling->child_pos[0];
    }

    this->data[index] = sibling->data[0];

    for (int i = 1; i < sibling->num; i++) {
        sibling->data[i-1] = sibling->data[i];
    }

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->num; ++i) {
            sibling->child[i-1] = sibling->child[i];
            sibling->child_pos[i-1] = sibling->child_pos[i];
        }
        //----------------------------------------------
        //sibling->child[sibling->num] = nullptr;
        //----------------------------------------------
    }

    child->num += 1;
    sibling->num -= 1;

    write_file(this->tree, this, this->pos);
    write_file(this->tree, child, child->pos);
    write_file(this->tree, sibling, sibling->pos);
}

void BTreeNode::merge(int index) {
    BTreeNode* child = this->child[index];
    BTreeNode* sibling = this->child[index + 1];

    child->data[this->degree - 1] = this->data[index];

    for (int i = 0; i < sibling->num; ++i) {
        child->data[i+this->degree] = sibling->data[i];
    }

    if (!child->leaf) {
        for (int i = 0; i <= sibling->num; ++i) {
            child->child[i+this->degree] = sibling->child[i];
            child->child_pos[i+this->degree] = sibling->child_pos[i];
        }
    }

    for (int i = index + 1; i < this->num; ++i) {
        this->data[i - 1] = this->data[i];
    }

    //----------------------------------
    //this->child[index + 1] = nullptr;
    //----------------------------------

    for (int i = index + 2; i <= this->num; ++i) {
        this->child[i-1] = this->child[i];
        this->child_pos[i-1] = this->child_pos[i];
    }
    //---------------------------------
    //this->child[this->num] = nullptr;
    //----------------------------------

    child->num += sibling->num + 1;

    this->num--;

    write_file(this->tree, this, this->pos);
    write_file(this->tree, child, child->pos);
    write_file(this->tree, sibling, sibling->pos);

    delete sibling;
}