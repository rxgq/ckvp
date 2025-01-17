#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h> 
#include <windows.h>

#include "src/kvp_store.h"
#include "src/kvp_tests.h"
#include "src/parser.h"
#include "src/kvp_engine.h"

const char *CKVP_FILE = "ckvp.dat";

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

char *get_base_path() {
    char *home_dir;
    char *default_path;

#ifdef _WIN32
    home_dir = getenv("USERPROFILE");
    if (!home_dir) {
        fprintf(stderr, "error setting up kvp store path: USERPROFILE environment variable not found.\n");
        exit(EXIT_FAILURE);
    }

    default_path = malloc(strlen(home_dir) + strlen("\\AppData\\Local\\ckvp") + 1);
    sprintf(default_path, "%s\\AppData\\Local\\ckvp", home_dir);
#else
    home_dir = getenv("HOME");
    if (!home_dir) {
        fprintf(stderr, "error setting up kvp store path: HOME environment variable not found.\n");
        exit(EXIT_FAILURE);
    }
    default_path = malloc(strlen(home_dir) + strlen("/.config/ckvp") + 1);
    sprintf(default_path, "%s/.config/ckvp", home_dir);
#endif

    return default_path;
}

char *get_full_path(char *base_path, const char *filename) {
    size_t path_len = strlen(base_path);
    size_t file_len = strlen(filename);

    char *full_path = (char *)malloc(path_len + file_len + 2);
    snprintf(full_path, path_len + file_len + 2, "%s\\%s", base_path, filename);

    return full_path;
}

int create_ckvp_dir(char *path) {
    if (_mkdir(path) != 0) {
        if (errno != EEXIST) {
            perror("Error creating directory: ");
            printf("%s", path);
            return 0;
        }
    }

    return 1;
}

int get_input(char *buffer, size_t buffer_size, const char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, buffer_size, stdin) == NULL) {
        printf("\nInput error.\n");
        return -1;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return 0;
}

void setup_auth(KvpStore *store) {
    printf("\n");
    while (1) {
        char password[64];
        char password_confirmation[64];

        if (get_input(password, sizeof(password), "password: ") != 0) {
            return;
        }
        if (strcmp(password, "") == 0) continue;

        if (get_input(password_confirmation, sizeof(password_confirmation), "confirm password: ") != 0) {
            return;
        }

        if (strcmp(password, password_confirmation) != 0) {
            printf("\n  Passwords did not match.\n\n");
            continue;
        }

        printf("\n  Password successfully set.\n");
        store->pass = strdup(password);
        break;
    }
}

int main(int argc, char *argv[]) {
    // run_tests();

    char *base_path = get_base_path();
    char *full_path = get_full_path(base_path, CKVP_FILE);

    printf("ckvp path: %s\n", full_path);

    if (!create_ckvp_dir(base_path)) {
        free(base_path);
        free(full_path);
        return 0;
    }

    KvpStore *store = init_kvp_store();

    FILE *fptr = fopen(full_path, "r");
    if (!fptr) {
        printf("\nC KVP setup required");

        if (!get_choice("  perform setup?")) {
            printf("\ndid not proceed with ckvp setup");
            fclose(fptr);
            return 0;
        }
    }
    fclose(fptr);

    if (!load_from_file(store, full_path)) {
        printf("\n  ckvp.dat does not already exist");
        save_to_file(store, full_path);
        printf("\n  created ckvp.dat");
    } else {
        printf("\ninitialising ckvp\n");
        printf("\n  found ckvp.dat");
        load_from_file(store, full_path);
        printf("\n  successfully loaded contents from ckvp.dat");
    }

    if (strcmp(store->pass, "") == 0) {
        if (get_choice("  \ncreate authorisation password? (recommended)")) {
            setup_auth(store);
            save_to_file(store, full_path);
        }
    }
    
    if (strcmp(store->pass, "") != 0) {
        printf("\n\n");
        while (1) {
            char input[64];
            get_input(input, 64, "password: ");

            if (strcmp(input, store->pass) == 0) {
                break;
            }
        }
    }

    printf("\n\nckvp console (type \"help\")");
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