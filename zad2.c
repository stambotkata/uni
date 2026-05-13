 #include <stdio.h>
 #include <string.h>
 #include <openssl/md5.h>
 

 void bytes_to_hex(unsigned char digest[MD5_DIGEST_LENGTH], char out[33]) {
     for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
         sprintf(out + i * 2, "%02x", digest[i]);
     out[32] = '\0';
 }
 
 int main(void) {
     const char *target = "e10adc3949ba59abbe56e057f20f883e";
     const char *salt   = "pwd_";
 
     char          input[16];             
     char          hex[33];
     unsigned char digest[MD5_DIGEST_LENGTH];
 
     printf("     MD5 Hash Cracker  –  Задача 2       \n");
     printf("Таргет хеш : %s\n", target);
     printf("Формат     : pwd_[a-z][a-z][a-z]\n");
     printf("Комбинации : 26^3 = 17 576\n");
     printf("Търся...\n\n");
 

     for (char c1 = 'a'; c1 <= 'z'; c1++) {
         for (char c2 = 'a'; c2 <= 'z'; c2++) {
             for (char c3 = 'a'; c3 <= 'z'; c3++) {
 
                 snprintf(input, sizeof(input), "%s%c%c%c", salt, c1, c2, c3);
 
                 MD5((unsigned char *)input, strlen(input), digest);
                 bytes_to_hex(digest, hex);

                 if (strcmp(hex, target) == 0) {
                     printf("       >>>  ПАРОЛА  НАМЕРЕНА!  <<<       \n");
                     printf("Пълен вход : %s\n", input);
                     printf("Парола     : %c%c%c\n", c1, c2, c3);
                     printf("MD5 хеш    : %s\n", hex);
                     return 0;
                 }
             }
         }
     }
 
     printf("Паролата не беше намерена.\n");
     return 1;
 }