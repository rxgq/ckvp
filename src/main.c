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

void assert_equal(int n1, int n2) {
    if (n1 != n2) {
        printf("Assertion failed: %d == %d", n1, n2);
    }
}

void test_retrieve() {
    Kvp *kvp = init_kvp();

    insert(kvp, 1024, 10);
    int n = retrieve(kvp, 1024);

    assert_equal(n, 10);
}

void test_retrieve_update() {
    Kvp *kvp = init_kvp();

    insert(kvp, 1024, 10);
    insert(kvp, 1024, 4);
    int n = retrieve(kvp, 1024);

    assert_equal(n, 4);
}

void test_delete() {
    Kvp *kvp = init_kvp();

    insert(kvp, 1024, 10);
    delete(kvp, 1024);

    int n = retrieve(kvp, 1024);

    assert_equal(n, -1);
}

int main(int argc, char *argv[]) {
    test_retrieve();
    test_retrieve_update();
    test_delete();

    return 0;
}

