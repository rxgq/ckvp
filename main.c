#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "src/kvp_store.h"
#include "src/kvp_tests.h"
#include "src/parser.h"
#include "src/kvp_engine.h"

int main(int argc, char *argv[]) {
    run_tests();

    KvpStore *store = init_kvp_store();

    if (load_from_file(store, "store.dat") == 1) {
        save_to_file(store, "store.dat");
    }

    load_from_file(store, "store.dat");

    while (1) {
        char input[256];
        printf("\n\n> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0) {
            break;
        }

        Symbol *symbols = parse_symbols(input);
        execute(store, symbols);
    }

    free(store->kvps);
    free(store);

    return 0;
}