#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kvp_engine.h"
#include "parser.h"
#include "kvp_store.h"

const char *TEMP_PATH = "C:\\Users\\rxgqq\\projects\\allium\\store\\store.txt";

KvpEngine *init_kvp_engine(Symbol *symbols) {
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

static void write_kvp(const char *key, const char *val) {
    FILE *fptr = fopen(TEMP_PATH, "a");

    fwrite(key, 1, strlen(key), fptr);
    fwrite(":", 1, 1, fptr);
    fwrite(val, 1, strlen(val), fptr);
    fwrite("\n", 1, 1, fptr);
    fclose(fptr);
}

static char* read_kvp(const char *key) {
    FILE *fptr = fopen(TEMP_PATH, "r");

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc(sz + 1);

    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';
    fclose(fptr);

    char *line = strtok(buff, "\n");
    while (line != NULL) {
        char *separator = strchr(line, ':');
        if (separator == NULL) break;

        *separator = '\0';
        char *file_key = line;
        char *file_val = separator + 1;

        if (strcmp(key, file_key) == 0) {
            free(buff);
            return strdup(file_val);
        }

        line = strtok(NULL, "\n");
    }

    free(buff);
    return NULL;
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

    write_kvp(key.lexeme, val.lexeme);
}


static void exec_get(KvpStore*store, KvpEngine *kvp_engine) {
    expect(kvp_engine, "get");

    Symbol key = current_symbol(kvp_engine);
    if (key.lexeme == NULL) {
        printf("expected key after 'get'");
        return;
    }
    advance(kvp_engine);

    char *val = read_kvp(key.lexeme);

    if (val == NULL) {
        printf("key '%s' not found", key.lexeme);
    } else {
        printf("Val: %s", val);
    }
}

static void exec_del(KvpStore*store, KvpEngine *kvp_engine) {
    expect(kvp_engine, "del");

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
    else {
        printf("Unknown command: %s", curr.lexeme);
        advance(kvp_engine);
    }
}

void execute(KvpStore *store, Symbol *symbols) {
    KvpEngine *kvp_engine = init_kvp_engine(symbols);

    while (symbols[kvp_engine->current].lexeme != NULL) {
        exec_symbol(store, kvp_engine);
    }
}