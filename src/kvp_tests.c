#include <stdio.h>
#include <string.h>

#include "kvp_store.h"
#include "parser.h"

int test_passed = 0;
int test_failed = 0;

void assert_equal(char *str1, char *str2, const char *test_name) {
    if (str1 == NULL && str2 == NULL) {
        test_passed++;
        return;
    }

    if (strcmp(str1, str2) != 0) {
        printf("Test '%s' failed: Expected '%s', but got '%s'\n", test_name, str2, str1);
        test_failed++;
    } else {
        test_passed++;
    }
}

void test_retrieve() {
    KvpStore *store = init_kvp_store();
    insert(store, "key1", "10");
    char *val = retrieve(store, "key1");
    assert_equal(val, "10", "test_retrieve");
}

void test_retrieve_update() {
    KvpStore *store = init_kvp_store();
    insert(store, "key1", "10");
    insert(store, "key1", "20");
    char *val = retrieve(store, "key1");
    assert_equal(val, "20", "test_retrieve_update");
}

void test_delete() {
    KvpStore *store = init_kvp_store();
    insert(store, "key1", "10");
    delete_kvp(store, "key1");
    char *val = retrieve(store, "key1");
    assert_equal(val, NULL, "test_delete");
}

void test_collision() {
    KvpStore *store = init_kvp_store();
    insert(store, "key1", "10");
    insert(store, "key2", "20");
    insert(store, "key3", "30");
    char *val1 = retrieve(store, "key1");
    char *val2 = retrieve(store, "key2");
    char *val3 = retrieve(store, "key3");
    assert_equal(val1, "10", "test_collision_key1");
    assert_equal(val2, "20", "test_collision_key2");
    assert_equal(val3, "30", "test_collision_key3");
}

void test_empty_table_retrieve() {
    KvpStore *store = init_kvp_store();
    char *val = retrieve(store, "nonexistent");
    assert_equal(val, NULL, "test_empty_table_retrieve");
}

void test_delete_non_existent_key() {
    KvpStore *store = init_kvp_store();
    insert(store, "key1", "10");
    delete_kvp(store, "key2");
    char *val = retrieve(store, "key1");
    assert_equal(val, "10", "test_delete_non_existent_key");
}

void test_reinsert_after_delete() {
    KvpStore *store = init_kvp_store();
    insert(store, "key1", "10");
    delete_kvp(store, "key1");
    char *val = retrieve(store, "key1");
    assert_equal(val, NULL, "test_reinsert_after_delete");
    insert(store, "key1", "20");
    val = retrieve(store, "key1");
    assert_equal(val, "20", "test_reinsert_after_delete");
}

void test_parser_input() {
    char *source = "set x 10";
    Symbol* symbols = parse_symbols(source);

    assert_equal("set", symbols[0].lexeme, "test_parser_input");
    assert_equal("x", symbols[1].lexeme, "test_parser_input");
    assert_equal("10", symbols[2].lexeme, "test_parser_input");
}

void test_parser_input_whitespace() {
    char *source = "    set                  x    10";
    Symbol* symbols = parse_symbols(source);

    assert_equal("set", symbols[0].lexeme, "test_parser_input_whitespace");
    assert_equal("x", symbols[1].lexeme, "test_parser_input_whitespace");
    assert_equal("10", symbols[2].lexeme, "test_parser_input_whitespace");
}

void run_tests() {
    test_retrieve();
    test_retrieve_update();
    test_delete();
    test_collision();
    test_empty_table_retrieve();
    test_delete_non_existent_key();
    test_reinsert_after_delete();
    test_parser_input();
    test_parser_input_whitespace();

    printf("\nTest Results:\n");
    printf("  Passed: %d\n", test_passed);
    printf("  Failed: %d\n\n", test_failed);

    if (test_failed == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed. Please check the output above.\n");
    }
}