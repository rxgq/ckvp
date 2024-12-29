#ifndef KVPSTORE_H
#define KVPSTORE_H

typedef struct Kvp {
    int key;
    int val;
} Kvp;

typedef struct KvpStore {
    int capacity;
    int count;
    Kvp *kvps;
} KvpStore;

extern void insert(KvpStore *store, int key, int val);
extern int retrieve(KvpStore *store, int key);
extern void delete_kvp(KvpStore *store, int key);
extern KvpStore *init_kvp_store();

#endif