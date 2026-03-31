#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
void print_histogram(const char *text) {
  int counts[26] = {0};
  int total_letters = 0;
  int biggest = 0;

  for (int i = 0; text[i] != '\0'; i++) {
    if (isalpha(text[i])) {
      int index = toupper(text[i]) - 'A';
      counts[index]++;
      total_letters++;
    }
  }
  for (int i = 0; i < 26; i++) {
    if (counts[i] > biggest)
      biggest = counts[i];
  }
  printf("\n--- FREQUENCY HISTOGRAM ---\n");
  for (int i = 0; i < 26; i++) {
    if (counts[i] > 0) {
      printf("%c (%3d): ", 'A' + i, counts[i]);
      for (int j = 0; j < counts[i]; j++) {
        printf("*");
      }
      printf("\n");
      if (counts[i] == biggest) {
        biggest = 'A' + i;
      }
    }
  }
  printf("Suggested keys: %d ,%d ,%d, %d", biggest - 'E',biggest - 'T',biggest - 'A',biggest - 'O');
}

int main() {
  FILE *fptr;
  char text[500];

  fptr = fopen("encrypted.txt", "r");
  fgets(text, sizeof(text), fptr);
  fclose(fptr);
  print_histogram(text);

  return EXIT_SUCCESS;
}
