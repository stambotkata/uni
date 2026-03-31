#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char caesar_encrypt_char(char p, int k) {
    if (isupper(p)) {
        return ((p - 'A' + k) % 26) + 'A';
    }
    else if (islower(p)) {
        return((p - 'a' + k) % 26) + 'a';     
    }
    return p;
}

char caesar_decrypt_char(char c, int k) {
    if (isupper(c)) {
        return ((c - 'A' - k + 26) % 26) + 'A'; 
    }
    else if (islower(c)) {
        return ((c - 'a' - k + 26) % 26) + 'a';
    }
    return c;
}

int main() {
    int a;
    char unencrypted[500];
    char encrypted[500];
    char decrypted[500];

    printf("What's the sentence you'd like to encrypt:\n");
    fgets(unencrypted, sizeof(unencrypted), stdin);
    
    printf("And by how many letters would you want to shift it with:\n");
    scanf("%d", &a);

    for (int i = 0; i < strlen(unencrypted); i++) {
        encrypted[i] = caesar_encrypt_char(unencrypted[i], a);
    }

    for (int i = 0; i < strlen(unencrypted); i++) {
        decrypted[i] = caesar_decrypt_char(encrypted[i], a);
    }

    printf("Original string: %s", unencrypted);
    printf("Encrypted string: %s", encrypted);
    printf("Decrypted string: %s", decrypted);

    return EXIT_SUCCESS;
}
