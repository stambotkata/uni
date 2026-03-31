#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char caesar_decrypt_char(char c, int k) {
    if (isupper(c)) {
        return ((c - 'A' - k + 26) % 26) + 'A';
    }
    else if (islower(c)) {
        return ((c - 'a' - k + 26) % 26) + 'a';
    }
    return c;
}
int main()
{
    FILE* fptr;
    char encrypted[500];
    char decrypted[500];
    fptr=fopen("encrypted.txt","r");
    fgets(encrypted, sizeof(encrypted), fptr);
    for(int a=0;a<25;a++)
    {
        for (int i = 0; i < strlen(encrypted); i++) 
        {
            decrypted[i] = caesar_decrypt_char(encrypted[i], a);
        }
        printf("Decrypted string: %s \n", decrypted);
    }
    return EXIT_SUCCESS;
}
