#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_SIZE 1024
#define EMPTY_KEY -1
#define DELETED_KEY -2

typedef struct Kvp {
    int key;
    int val;
} Kvp;

unsigned int hash(int key) {
    return key % TABLE_SIZE;
}

void insert(Kvp* kvp, int key, int val) {
    unsigned int index = hash(key);
    
    while (kvp[index].key != EMPTY_KEY && kvp[index].key != DELETED_KEY) {
        if (kvp[index].key == key) {
            kvp[index].val = val;
            return;
        }

        index = hash(index + 1);
    }

    kvp[index].key = key;
    kvp[index].val = val;
}

int retrieve(Kvp* kvp, int key) {
    unsigned int index = hash(key);

    while (kvp[index].key != EMPTY_KEY) {
        if (kvp[index].key == key) {
            return kvp[index].val;
        }

        index = hash(index + 1);
    }

    return -1;
}

void delete(Kvp *kvp, int key) {
    unsigned int index = hash(key);

    while (kvp[index].key != EMPTY_KEY) {
        if (kvp[index].key == key) {
            kvp[index].key = DELETED_KEY;
        }

        index = hash(index + 1);
    }
}

Kvp *init_kvp() {
    Kvp *kvp = (Kvp *)malloc(sizeof(Kvp) * TABLE_SIZE);

    for (int i = 0; i < TABLE_SIZE; i++) {
        kvp[i].key = EMPTY_KEY;
    }

    return kvp;
}

int main(int argc, char *argv[]) {

    return 0;
}

