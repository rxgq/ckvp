#include <stdlib.h>
#include "kvp_store.h"

#define TABLE_SIZE 1024
#define EMPTY_KEY -1
#define DELETED_KEY -2

static unsigned int hash(int key) {
    return key % TABLE_SIZE;
}

void insert(KvpStore* store, int key, int val) {
    unsigned int index = hash(key);

    if (key == EMPTY_KEY || key == DELETED_KEY) {
        return;
    }

    if (store->count == store->capacity) {
        return;
    }
    
    while (store->kvps[index].key != EMPTY_KEY && store->kvps[index].key != DELETED_KEY) {
        if (store->kvps[index].key == key) {
            store->kvps[index].val = val;
            return;
        }

        index = hash(index + 1);
    }

    store->kvps[index].key = key;
    store->kvps[index].val = val;

    store->count++;
}

int retrieve(KvpStore* store, int key) {
    if (key < 0) {
        return -1;
    }

    unsigned int index = hash(key);

    while (store->kvps[index].key != EMPTY_KEY) {
        if (store->kvps[index].key == key) {
            return store->kvps[index].val;
        }

        index = hash(index + 1);
    }

    return -1;
}

void delete_kvp(KvpStore *store, int key) {
    unsigned int index = hash(key);

    while (store->kvps[index].key != EMPTY_KEY) {
        if (store->kvps[index].key == key) {
            store->kvps[index].key = DELETED_KEY;
        }

        index = hash(index + 1);
    }
}

static Kvp *init_kvp() {
    Kvp *kvp = (Kvp *)malloc(sizeof(Kvp) * TABLE_SIZE);

    for (int i = 0; i < TABLE_SIZE; i++) {
        kvp[i].key = EMPTY_KEY;
    }

    return kvp;
}

KvpStore *init_kvp_store() {
    KvpStore *store = (KvpStore *)malloc(sizeof(KvpStore));
    store->kvps = init_kvp();
    store->capacity = TABLE_SIZE;
    store->count = 0;

    return store;
}