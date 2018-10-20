//
// Created by lap11852 on 21/09/2018.
//

#ifndef KEYVALUESTORE_DATASTORE_H
#define KEYVALUESTORE_DATASTORE_H

#include "Global.h"

class DataStore {
protected:

public:
    char key[1024];
    char value[1024];
    DataStore() {

    }
    DataStore(KEY key, VALUE value) {
        strcpy(this->key, key);
        strcpy(this->value, value);
    }
    DataStore& operator = (const DataStore& data) {
        if (this != &data) {
            strcpy(this->key, data.key);
            strcpy(this->value, data.value);
        }
        return *this;
    }

    friend  ostream &operator << (ostream &output, const DataStore &dataStore) {
        output << "Key: " << dataStore.key << " Value: " << dataStore.value;
        return output;
    }

    friend bool operator < (const DataStore& value1, const DataStore& value2) {
        return string(value1.key) < string(value2.key);
    }

    friend bool operator > (const DataStore& value1, const DataStore& value2) {
        return string(value1.key) > string(value2.key);
    }

    friend bool operator == (const DataStore& value1, const DataStore& value2) {
        return string(value1.key) == string(value2.key);
    }

    friend class BTreeNode;
    friend class BTree;
};


#endif //KEYVALUESTORE_DATASTORE_H
