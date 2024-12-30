#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kvp_store.h"

#define MAX_KEY_LEN 256
#define MAX_VALUE_LEN 1024
#define STORE_SIZE 100

#define INITIAL_TABLE_SIZE 1000

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
            printf("Updated value for key: %s\n", key);
            return;
        }

        index = (index + 1) % STORE_SIZE;
    }

    strncpy(store->kvps[index].key, key, MAX_KEY_LEN);
    store->kvps[index].key[MAX_KEY_LEN - 1] = '\0';
    strncpy(store->kvps[index].val, val, MAX_VALUE_LEN);
    store->kvps[index].val[MAX_VALUE_LEN - 1] = '\0';

    store->count++;
    printf("Inserted key: %s, value: %s\n", key, val);
}

char *retrieve(KvpStore* store, char *key) {
    unsigned int index = hash(store, key);

    while (store->kvps[index].key[0] != '\0') {
        if (strcmp(store->kvps[index].key, key) == 0) {
            printf("Retrieved: %s", store->kvps[index].val);
            return store->kvps[index].val;
        }

        index = (index + 1) % STORE_SIZE;
    }

    return NULL;
}

void delete_kvp(KvpStore *store, char *key) {
    unsigned int index = hash(store, key);

    while (store->kvps[index].key > 0) {
        if (store->kvps[index].key == key) {
            memset(&store->kvps[index], 0, sizeof(Kvp));
            store->count--;
            return;
        }

        index = (index + 1) % STORE_SIZE;
    }
}

static Kvp *init_kvp() {
    Kvp *kvp = (Kvp *)malloc(sizeof(Kvp) * INITIAL_TABLE_SIZE);
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
    store->capacity = INITIAL_TABLE_SIZE;
    store->count = 0;

    return store;
}