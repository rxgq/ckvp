#include <stdlib.h>
#include <stdio.h>

#include "kvp_store.h"

#define INITIAL_TABLE_SIZE 1024
#define EMPTY_KEY -1
#define DELETED_KEY -2

static inline unsigned int hash(KvpStore *store, int key) {
    return key % store->capacity;
}

void insert(KvpStore* store, int key, int val) {
    if (key == EMPTY_KEY || key == DELETED_KEY) {
        return;
    }

    if (store->count == store->capacity) {
        return;
    }
    
    unsigned int index = hash(store, key);

    while (store->kvps[index].key != EMPTY_KEY && store->kvps[index].key != DELETED_KEY) {
        if (store->kvps[index].key == key) {
            store->kvps[index].val = val;
            return;
        }

        index = hash(store, index + 1);
    }

    store->kvps[index].key = key;
    store->kvps[index].val = val;

    store->count++;
}

int retrieve(KvpStore* store, int key) {
    if (key < 0) return -1;

    unsigned int index = hash(store, key);

    while (store->kvps[index].key != EMPTY_KEY) {
        if (store->kvps[index].key == key) {
            return store->kvps[index].val;
        }

        index = hash(store, index + 1);
    }

    return -1;
}

void delete_kvp(KvpStore *store, int key) {
    unsigned int index = hash(store, key);

    while (store->kvps[index].key != EMPTY_KEY) {
        if (store->kvps[index].key == key) {
            store->kvps[index].key = DELETED_KEY;
            store->count--;
        }

        index = hash(store, index + 1);
    }
}

static Kvp *init_kvp() {
    Kvp *kvp = (Kvp *)malloc(sizeof(Kvp) * INITIAL_TABLE_SIZE);
    if (!kvp) {
        printf("Memory allocation failed for kvp\n");
        exit(1);
    }
    
    for (int i = 0; i < INITIAL_TABLE_SIZE; i++) {
        kvp[i].key = EMPTY_KEY;
    }
    return kvp;
}

KvpStore *init_kvp_store() {
    KvpStore *store = (KvpStore *)malloc(sizeof(KvpStore));
    if (!store) {
        printf("Memory allocation failed for KvpStore\n");
        exit(1);
    }

    store->kvps = init_kvp();
    store->capacity = INITIAL_TABLE_SIZE;
    store->count = 0;

    return store;
}