#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"

char *symbol_map[] = {
    "get",
    "set",
    "del"
};

static Parser *init_parser(char *source) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->symbols = (Symbol *)malloc(sizeof(Symbol) * 2);
    parser->source = source;
    parser->symbol_capacity = 2;
    parser->current = 0;
    parser->symbol_count = 0;

    return parser;
}

static inline void advance(Parser *parser) {
    parser->current++;
}

static inline char current_char(Parser *parser) {
    return parser->source[parser->current];
}

static void create_symbol(Parser *parser, char *lexeme) {
    if (parser->symbol_count >= parser->symbol_capacity) {
        parser->symbol_capacity *= 2;
        parser->symbols = realloc(parser->symbols, sizeof(Symbol) * parser->symbol_capacity);
    }

    Symbol *symbol = &parser->symbols[parser->symbol_count++];
    symbol->lexeme = strdup(lexeme);
}


static void parse_identifier(Parser *parser) {
    int start = parser->current;

    while (current_char(parser) != '\0' && isalpha(current_char(parser))) {
        advance(parser);
    }

    int len = parser->current - start;

    char* lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, parser->source + start, len);
    lexeme[len] = '\0';

    create_symbol(parser, lexeme);
    free(lexeme);
}

static void parse_numeric(Parser *parser) {
    int start = parser->current;

    while (current_char(parser) != '\0' && isdigit(current_char(parser))) {
        advance(parser);
    }

    int len = parser->current - start;

    char* lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, parser->source + start, len);
    lexeme[len] = '\0';

    create_symbol(parser, lexeme);
    free(lexeme);
}

static void parse_symbol(Parser *parser) {
    char curr = parser->source[parser->current];
    
    if (isalpha(curr)) {
        parse_identifier(parser);
    }
    else if (isdigit(curr)) {
        parse_numeric(parser);
    }
}

Symbol *parse_symbols(char *source) {
    Parser *parser = init_parser(source);

    while (parser->source[parser->current] != '\0') {
        parse_symbol(parser);
        advance(parser);
    }

    create_symbol(parser, NULL);

    return parser->symbols;
}