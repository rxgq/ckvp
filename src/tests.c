#include <stdio.h>
#include "kvp_store.h"

int test_passed = 0;
int test_failed = 0;

void assert_equal(int n1, int n2, const char *test_name) {
    if (n1 != n2) {
        printf("Test '%s' failed: Expected %d, but got %d\n", test_name, n2, n1);
        test_failed++;
    } else {
        test_passed++;
    }
}

void test_retrieve() {
    KvpStore *store = init_kvp_store();
    insert(store, 1024, 10);
    int n = retrieve(store, 1024);
    assert_equal(n, 10, "test_retrieve");
}

void test_retrieve_update() {
    KvpStore *store = init_kvp_store();
    insert(store, 1024, 10);
    insert(store, 1024, 4);
    int n = retrieve(store, 1024);
    assert_equal(n, 4, "test_retrieve_update");
}

void test_delete() {
    KvpStore *store = init_kvp_store();
    insert(store, 1024, 10);
    delete_kvp(store, 1024);
    int n = retrieve(store, 1024);
    assert_equal(n, -1, "test_delete");
}

void test_collision() {
    KvpStore *store = init_kvp_store();
    insert(store, 1024, 10);
    insert(store, 2048, 20);
    insert(store, 3072, 30);
    int n1 = retrieve(store, 1024);
    int n2 = retrieve(store, 2048);
    int n3 = retrieve(store, 3072);
    assert_equal(n1, 10, "test_collision_1024");
    assert_equal(n2, 20, "test_collision_2048");
    assert_equal(n3, 30, "test_collision_3072");
}

void test_empty_table_retrieve() {
    KvpStore *store = init_kvp_store();
    int n = retrieve(store, 42);
    assert_equal(n, -1, "test_empty_table_retrieve");
}

void test_delete_non_existent_key() {
    KvpStore *store = init_kvp_store();
    insert(store, 1024, 10);
    delete_kvp(store, 2048);
    int n = retrieve(store, 1024);
    assert_equal(n, 10, "test_delete_non_existent_key");
}

void test_insert_reserved_key() {
    KvpStore *store = init_kvp_store();
    insert(store, -1, 10);
    int n = retrieve(store, -1);
    assert_equal(n, -1, "test_insert_reserved_key");
}

void test_hash_table_max_capacity() {
    KvpStore *store = init_kvp_store();
    for (int i = 0; i < store->capacity + 1; i++) {
        insert(store, i, i);
    }
    // if it gets to this point it did not get stuck in an infinite loop of linear probing
    assert_equal(1, 1, "test_hash_table_max_capacity");
}

void test_reinsert_after_delete() {
    KvpStore *store = init_kvp_store();
    insert(store, 1024, 10);
    delete_kvp(store, 1024);
    int n = retrieve(store, 1024);
    assert_equal(n, -1, "test_reinsert_after_delete");
    insert(store, 1024, 20);
    n = retrieve(store, 1024);
    assert_equal(n, 20, "test_reinsert_after_delete");
}

void run_tests() {
    test_retrieve();
    test_retrieve_update();
    test_delete();
    test_collision();
    test_empty_table_retrieve();
    test_delete_non_existent_key();
    test_insert_reserved_key();
    test_hash_table_max_capacity();
    test_reinsert_after_delete();

    printf("\nTest Results:\n");
    printf("Tests Passed: %d\n", test_passed);
    printf("Tests Failed: %d\n", test_failed);

    if (test_failed == 0) {
        printf("All tests passed!\n");
    } else {
        printf("Some tests failed. Please check the output above.\n");
    }
}