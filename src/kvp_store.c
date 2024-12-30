#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kvp_store.h"

#define MAX_KEY_LEN 256
#define MAX_VALUE_LEN 1024
#define STORE_SIZE 100

unsigned int hash(KvpStore *store, const char* key) {
    unsigned int hash_value = 0;
    while (*key) {
        hash_value = (hash_value * 31) + *key++;
    }

    return hash_value % STORE_SIZE;
}

void insert(KvpStore* store, char *key, char *val) {
    if (store->count >= store->capacity) {
        printf("Kvp Store is full.\n");
        return;
    }
    
    unsigned int index = hash(store, key);

    while (store->kvps[index].key[0] != '\0') {
        if (strcmp(store->kvps[index].key, key) == 0) {
            strncpy(store->kvps[index].val, val, MAX_VALUE_LEN);
            store->kvps[index].val[MAX_VALUE_LEN - 1] = '\0';
            return;
        }

        index = (index + 1) % STORE_SIZE;
    }

    strncpy(store->kvps[index].key, key, MAX_KEY_LEN);
    store->kvps[index].key[MAX_KEY_LEN - 1] = '\0';
    strncpy(store->kvps[index].val, val, MAX_VALUE_LEN);
    store->kvps[index].val[MAX_VALUE_LEN - 1] = '\0';

    store->count++;
}

char *retrieve(KvpStore* store, char *key) {
    unsigned int index = hash(store, key);

    while (store->kvps[index].key[0] != '\0') {
        if (strcmp(store->kvps[index].key, key) == 0) {
            return store->kvps[index].val;
        }

        index = (index + 1) % STORE_SIZE;
    }

    return NULL;
}

void delete_kvp(KvpStore *store, char *key) {
    unsigned int index = hash(store, key);

    while (store->kvps[index].key[0] != '\0') {
        if (strcmp(store->kvps[index].key, key) == 0) {
            memset(&store->kvps[index], 0, sizeof(Kvp));
            store->count--;
            return;
        }

        index = (index + 1) % store->capacity;
    }
}

Kvp *get_all(KvpStore *store, int *count) {
    Kvp *all_kvps = (Kvp *)malloc(sizeof(Kvp) * store->count);
    if (!all_kvps) {
        printf("Memory allocation failed for all key-value pairs.\n");
        exit(1);
    }

    int index = 0;
    for (int i = 0; i < STORE_SIZE; i++) {
        if (store->kvps[i].key[0] != '\0') {
            strncpy(all_kvps[index].key, store->kvps[i].key, MAX_KEY_LEN);
            strncpy(all_kvps[index].val, store->kvps[i].val, MAX_VALUE_LEN);
            index++;
        }
    }

    *count = index;

    return all_kvps;
}



static Kvp *init_kvp() {
    Kvp *kvp = (Kvp *)malloc(sizeof(Kvp) * STORE_SIZE);
    if (!kvp) {
        printf("Memory allocation failed for kvp\n");
        exit(1);
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
    store->capacity = STORE_SIZE;
    store->count = 0;
    store->pass = (char *)malloc(64);
    store->pass[0] = '\0';

    return store;
}