#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h> 

#include "src/kvp_store.h"
#include "src/kvp_tests.h"
#include "src/parser.h"
#include "src/kvp_engine.h"

const char *ALLIUM_PATH = "C:\\Users\\rxgqq\\projects\\allium\\allium";
const char *ALLIUM_FILE = "allium.dat";

int get_choice(const char *message) {
    char input[64];

    printf("\n");
    while (1) {
        printf("%s (y/n): ", message);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nInput error.\n");
            return 0;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "y") == 0) {
            return 1;
        } else if (strcmp(input, "n") == 0) {
            return 0;
        } 
    }
}

char *get_full_path() {
    size_t path_len = strlen(ALLIUM_PATH);
    size_t file_len = strlen(ALLIUM_FILE);

    char *full_path = (char *)malloc(path_len + file_len + 2);

    snprintf(full_path, path_len + file_len + 2, "%s\\%s", ALLIUM_PATH, ALLIUM_FILE);

    return full_path;
}

int create_allium_dir() {
    if (_mkdir(ALLIUM_PATH) != 0) {
        if (errno != EEXIST) {
            perror("Error creating directory");
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    // run_tests();

    char *full_path = get_full_path();
    KvpStore *store = init_kvp_store();

    FILE *fptr = fopen(full_path, "r");
    if (!fptr) {
        printf("\nAllium KVP setup required");

        if (!get_choice("  perform setup?")) {
            printf("\ndid not proceed with allium setup");
            fclose(fptr);
            return 0;
        }

        create_allium_dir();
    }
    fclose(fptr);

    if (!load_from_file(store, full_path)) {
        printf("\n  allium.dat does not exist");
        save_to_file(store, full_path);
        printf("\n  created allium.dat");
    } else {
        printf("\n  found allium.dat");
        load_from_file(store, full_path);
        printf("\n  successfully loaded contents from allium.dat");
    }

    printf("\n\n\nallium console (type \"help\")");
    while (1) {
        char input[256];
        printf("\n> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "quit") == 0 || strcmp(input, "q") == 0) {
            break;
        }

        Symbol *symbols = parse_symbols(input);

        execute(store, symbols);
        printf("\n");

        save_to_file(store, full_path);
    }

    free(store->kvps);
    free(store);

    return 0;
}