#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void xor_cipher(char *text) {
  int len = strlen(text);
  char key = 'K';
  for (int i = 0; i < len; i++) {
    text[i] = text[i] ^ key;
  }
}
int main() {
  char data[] = "Secret";

  xor_cipher(data);
  printf("Encrypted: %s\n",
         data); 
  xor_cipher(data);
  printf("Decrypted: %s\n", data);

  return EXIT_SUCCESS;
}
