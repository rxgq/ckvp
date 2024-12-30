#include "symbol.h"
#include "kvp_store.h"

#ifndef ENGINE_H
#define ENGINE_H

typedef struct KvpEngine {
    Symbol *symbols;
    int symbol_count;
    int current;
} KvpEngine;

extern void execute(KvpStore *store, Symbol *symbols);

#endif