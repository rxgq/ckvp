#ifndef KVPSTORE_H
#define KVPSTORE_H

typedef struct Kvp {
    char key[256];
    char val[1024];
} Kvp;

typedef struct KvpStore {
    int capacity;
    int count;
    Kvp *kvps;
    char *pass;
} KvpStore;

extern void insert(KvpStore *store, char *key, char * val);
extern char* retrieve(KvpStore *store, char * key);
extern void delete_kvp(KvpStore *store, char * key);
extern KvpStore *init_kvp_store();
extern unsigned int hash(KvpStore *store, const char* key);

#endif