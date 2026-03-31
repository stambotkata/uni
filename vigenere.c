#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void vigenere_encrypt(const char *plaintext, const char *key, char *output) {
  int key_len = strlen(key);
  int key_index = 0;

  for (int i = 0; plaintext[i] != '\0'; i++) {
    char p = plaintext[i];

    if (isalpha(p)) {
      int shift = toupper(key[key_index % key_len]) - 'A';

      if (isupper(p)) {
        output[i] = ((p - 'A' + shift) % 26) + 'A';
      } else if (islower(p)) {
        output[i] = ((p - 'a' + shift) % 26) + 'a';
      }

      key_index++;
    } else {
      output[i] = p;
    }
  }
  output[strlen(plaintext)] = '\0';
}

void vigenere_decrypt(const char *plaintext, const char *key, char *output) {
  int key_len = strlen(key);
  int key_index = 0;

  for (int i = 0; plaintext[i] != '\0'; i++) {
    char p = plaintext[i];

    if (isalpha(p)) {
      int shift = toupper(key[key_index % key_len]) - 'A';

      if (isupper(p)) {
        output[i] = ((p - 'A' - shift + 26) % 26) + 'A';
      } else if (islower(p)) {
        output[i] = ((p - 'a' - shift + 26) % 26) + 'a';
      }

      key_index++;
    } else {
      output[i] = p;
    }
  }
  output[strlen(plaintext)] = '\0';
}

int main() {
  FILE *fptr1, *fptr2, *fptr3;
  char input[500], output[500], key[500];

  fptr1 = fopen("plaintext.txt", "r");
  fgets(input, sizeof(input), fptr1);
  fclose(fptr1);

  fptr2 = fopen("key.txt", "r");
  fgets(key, sizeof(key), fptr2);
  key[strcspn(key, "\n")] = '\0';
  fclose(fptr2);

  vigenere_encrypt(input, key, output);
  fptr3 = fopen("encrypted.txt", "w");
  fputs(output, fptr3);
  fclose(fptr3);

  printf("%s \n", input);
  printf("%s \n", output);

  vigenere_decrypt(output, key, output);
  printf("%s", output);
  return EXIT_SUCCESS;
}