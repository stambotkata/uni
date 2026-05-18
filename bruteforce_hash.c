#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

int main() {
    const char *target = "9daae68368ac7be2d9902bb79b556c04";
    const char *salt   = "LAB_";

    for (int pin = 0; pin <= 9999; pin++) {

        char input[16];
        snprintf(input, sizeof(input), "%s%04d", salt, pin);

        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_len;

        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
        EVP_DigestUpdate(ctx, input, strlen(input));
        EVP_DigestFinal_ex(ctx, digest, &digest_len);
        EVP_MD_CTX_free(ctx);

        char hex[33];
        for (int i = 0; i < 16; i++)
            sprintf(hex + 2*i, "%02x", digest[i]);
        hex[32] = '\0';

        if (strcmp(hex, target) == 0) {
            printf("Намерен ПИН: %04d\n", pin);
            printf("Вход:        %s\n", input);
            printf("MD5 хеш:     %s\n", hex);
            EVP_MD_CTX_free(ctx);
            return 0;
        }
    }

    printf("ПИН не е намерен.\n");
    return 0;
}