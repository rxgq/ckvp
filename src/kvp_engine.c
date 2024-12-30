#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kvp_engine.h"
#include "parser.h"
#include "kvp_store.h"

static void expect(KvpEngine *kvp_engine, char *expected) {
    char* actual = kvp_engine->symbols[kvp_engine->current].lexeme;

    if (strcmp(actual, expected) != 0) {
        printf("Expected %s, got %s", expected, actual);
    } else {
        kvp_engine->current++;
    }
}

static inline void advance(KvpEngine *kvp_engine) {
    kvp_engine->current++;
}

static Symbol current_symbol(KvpEngine *kvp_engine) {
    return kvp_engine->symbols[kvp_engine->current];
}

void save_to_file(KvpStore *kvStore, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("\nError saving Kvp");
        file = fopen(filename, "w");
    }

    fwrite(&kvStore->count, sizeof(int), 1, file);
    fwrite(kvStore->kvps, sizeof(Kvp), 100, file);
    fwrite(kvStore->pass, sizeof(char), 64, file);

    fclose(file);
}

int load_from_file(KvpStore *kvStore, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return 0;
    }

    fread(&kvStore->count, sizeof(int), 1, file);
    fread(kvStore->kvps, sizeof(Kvp), 100, file);
    fread(kvStore->pass, sizeof(char), 64, file);

    fclose(file);
    return 1;
}

static void exec_set(KvpStore *store, KvpEngine *kvp_engine) {
    expect(kvp_engine, "set");

    Symbol key = current_symbol(kvp_engine);
    if (key.lexeme == NULL) {
        printf("expected key after 'set'");
        return;
    }
    advance(kvp_engine);

    Symbol val = current_symbol(kvp_engine);
    if (val.lexeme == NULL) {
        printf("expected value after 'set %s'", key.lexeme);
        return;
    }
    advance(kvp_engine);

    insert(store, key.lexeme, val.lexeme);
    printf("%s: %s", key.lexeme, val.lexeme);
}


static void exec_get(KvpStore *store, KvpEngine *kvp_engine) {
    expect(kvp_engine, "get");

    Symbol key = current_symbol(kvp_engine);
    if (key.lexeme == NULL) {
        printf("expected key after 'get'");
        return;
    }
    advance(kvp_engine);

    char *val = retrieve(store, key.lexeme);
    printf("%s", val);
}

static void exec_del(KvpStore *store, KvpEngine *kvp_engine) {
    expect(kvp_engine, "del");

    Symbol key = current_symbol(kvp_engine);
    if (key.lexeme == NULL) {
        printf("expected key after 'del'");
        return;
    }
    advance(kvp_engine);

    char *val = retrieve(store, key.lexeme);
    if (val == NULL) {
        printf("(null)");
        return;
    }

    delete_kvp(store, key.lexeme);
    printf("%s", key.lexeme);
}

static void exec_get_all(KvpStore *store, KvpEngine *kvp_engine) {
    expect(kvp_engine, "getall");

    int count = 0;
    Kvp* kvps = get_all(store, &count);

    for (int i = 0; i < count; i++) {
        printf("%s ", kvps[i].key);
        printf("%s", kvps[i].val);
        if (i != count - 1) printf("\n");
    }
}

static void exec_help(KvpEngine *kvp_engine) {
    printf("\nAvailable Commands:\n");
    printf("--------------------------------\n");
    printf("set <key> <value>\n");
    printf("  - Inserts a new key-value pair or updates an existing key with the specified value.\n");

    printf("get <key>\n");
    printf("  - Retrieves the value associated with the specified key.\n");

    printf("del <key>\n");
    printf("  - Deletes the specified key-value pair from the store.\n");

    printf("quit / q\n");
    printf("  - Exits the program.\n");

    printf("\nNotes:\n");
    printf("- Keys and values should be strings.\n");
    printf("- All changes are persistent acrsoss sessions, saved to a file.\n");
    printf("--------------------------------\n");

    advance(kvp_engine);
}

static void exec_symbol(KvpStore *store, KvpEngine *kvp_engine) {
    Symbol curr = current_symbol(kvp_engine);

    if (strcmp(curr.lexeme, "set") == 0) {
        exec_set(store, kvp_engine);
    }
    else if (strcmp(curr.lexeme, "get") == 0 ) {
        exec_get(store, kvp_engine);
    }
    else if (strcmp(curr.lexeme, "del") == 0 ) {
        exec_del(store, kvp_engine);
    }
    else if (strcmp(curr.lexeme, "help") == 0) {
        exec_help(kvp_engine);
    }
    else if (strcmp(curr.lexeme, "getall") == 0) {
        exec_get_all(store, kvp_engine);
    }
    else {
        printf("Unknown command: %s", curr.lexeme);
        advance(kvp_engine);
    }
}

static KvpEngine *init_kvp_engine(Symbol *symbols) {
    KvpEngine *kvp_engine = (KvpEngine *)malloc(sizeof(KvpEngine));

    int symbol_count = 0;
    while (symbols[symbol_count].lexeme != NULL) {
        symbol_count++;
    }
    kvp_engine->symbols = (Symbol *)malloc(sizeof(Symbol) * symbol_count);
    kvp_engine->symbols = symbols;

    kvp_engine->symbol_count = symbol_count;
    kvp_engine->current = 0;

    return kvp_engine;
}

void execute(KvpStore *store, Symbol *symbols) {
    KvpEngine *kvp_engine = init_kvp_engine(symbols);

    while (symbols[kvp_engine->current].lexeme != NULL) {
        exec_symbol(store, kvp_engine);
    }
}