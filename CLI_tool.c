#include <ctype.h>
#include <stdio.h>
#include <string.h>
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
    FILE *fptr; 
    int a;
    char unencrypted[500];
    char encrypted[500];
    char decrypted[500];

    fptr = fopen("plaintext.txt", "r");
    fgets(unencrypted, sizeof(unencrypted), fptr);
    fclose(fptr);
    
    fptr = fopen("key.txt","r"); 
    fscanf(fptr, "%d", &a);
    fclose(fptr);

    for (int i = 0; i < strlen(unencrypted); i++) {
        encrypted[i] = caesar_encrypt_char(unencrypted[i], a);
    }

    for (int i = 0; i < strlen(unencrypted); i++) {
        decrypted[i] = caesar_decrypt_char(encrypted[i], a);
    }

    printf("Original string: %s \n", unencrypted);
    printf("Encrypted string: %s \n", encrypted);
    printf("Decrypted string: %s \n", decrypted);

    fptr = fopen("encrypted.txt","w");
    fprintf(fptr, encrypted);
    fclose(fptr);

    fptr = fopen("decrypted.txt","w");
    fprintf(fptr, decrypted);
    fclose(fptr);
    
    return EXIT_SUCCESS;
}
