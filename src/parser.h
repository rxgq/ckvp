#include "symbol.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct Parser {
    char *source;
    Symbol *symbols;
    int current;
    int symbol_count;
    int symbol_capacity;
} Parser;

extern Symbol* parse_symbols(char *source);

#endif