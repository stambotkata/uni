#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>

#define MAX_LINE   512
#define MAX_USERS  64
#define MAX_WORD   256

typedef struct {
    char username[MAX_WORD];
    char hash[MAX_LINE];
    char cracked[MAX_WORD];
    char method[32];
} User;

static int parse_shadow_line(char *line, User *u) {
    char *username = strtok(line, ":");
    char *hash     = strtok(NULL, ":");
    if (!username || !hash) return 0;
    strncpy(u->username, username, sizeof(u->username) - 1);
    strncpy(u->hash,     hash,     sizeof(u->hash)     - 1);
    u->cracked[0] = '\0';
    u->method[0]  = '\0';
    return 1;
}

static int dictionary_attack(User *u, const char *dict_file) {
    FILE *f = fopen(dict_file, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open dictionary '%s'.\n", dict_file);
        return 0;
    }
    char word[MAX_WORD];
    while (fgets(word, sizeof(word), f)) {
        word[strcspn(word, "\n")] = '\0';
        char *computed = crypt(word, u->hash);
        if (computed && strcmp(computed, u->hash) == 0) {
            strncpy(u->cracked, word, sizeof(u->cracked) - 1);
            strncpy(u->method, "dictionary", sizeof(u->method) - 1);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

static int pin_bruteforce(User *u) {
    char guess[5];
    for (int i = 0; i <= 9999; i++) {
        sprintf(guess, "%04d", i);
        char *computed = crypt(guess, u->hash);
        if (computed && strcmp(computed, u->hash) == 0) {
            strncpy(u->cracked, guess, sizeof(u->cracked) - 1);
            strncpy(u->method, "brute-force", sizeof(u->method) - 1);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <shadow.txt> <dictionary.txt>\n", argv[0]);
        return 1;
    }

    const char *shadow_file = argv[1];
    const char *dict_file   = argv[2];

    FILE *sf = fopen(shadow_file, "r");
    if (!sf) {
        fprintf(stderr, "Error: Cannot open shadow file '%s'.\n", shadow_file);
        return 1;
    }

    User users[MAX_USERS];
    int count = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), sf) && count < MAX_USERS) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;
        char copy[MAX_LINE];
        strncpy(copy, line, sizeof(copy) - 1);
        if (parse_shadow_line(copy, &users[count]))
            count++;
    }
    fclose(sf);

    printf("Loaded %d user(s) from '%s'.\n\n", count, shadow_file);

    for (int i = 0; i < count; i++) {
        printf("[*] Attacking user: %s\n", users[i].username);

        printf("    Running dictionary attack...\n");
        if (dictionary_attack(&users[i], dict_file)) {
            printf("    [FOUND] %s -> %s (dictionary)\n",
                   users[i].username, users[i].cracked);
            continue;
        }

        printf("    Dictionary failed. Running PIN brute-force (0000-9999)...\n");
        if (pin_bruteforce(&users[i])) {
            printf("    [FOUND] %s -> %s (brute-force)\n",
                   users[i].username, users[i].cracked);
        } else {
            printf("    [NOT CRACKED]\n");
        }
    }

    printf("\n--- Results ---\n");
    for (int i = 0; i < count; i++) {
        if (strlen(users[i].cracked) > 0)
            printf("  %-12s: %s (%s)\n",
                   users[i].username, users[i].cracked, users[i].method);
        else
            printf("  %-12s: [NOT CRACKED]\n", users[i].username);
    }

    return 0;
}
