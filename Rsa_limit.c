#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#define KEY_BITS 2048
#define MAX_PAYLOAD 245

int rsa_encrypt(EVP_PKEY *pubkey, const unsigned char *plaintext, size_t pt_len,
                unsigned char **ct_out, size_t *ct_len_out) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx) return 0;

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    size_t ct_len = 0;
    if (EVP_PKEY_encrypt(ctx, NULL, &ct_len, plaintext, pt_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    unsigned char *ct = malloc(ct_len);
    if (!ct) {
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    if (EVP_PKEY_encrypt(ctx, ct, &ct_len, plaintext, pt_len) <= 0) {
        free(ct);
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    *ct_out = ct;
    *ct_len_out = ct_len;
    EVP_PKEY_CTX_free(ctx);
    return 1;
}

int main() {
    printf("=== Exercise 6 Homework: RSA Size Limitation ===\n\n");

    printf("[1] Generating %d-bit RSA key pair...\n", KEY_BITS);

    EVP_PKEY_CTX *kctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(kctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(kctx, KEY_BITS);

    EVP_PKEY *pkey = NULL;
    if (EVP_PKEY_keygen(kctx, &pkey) <= 0) {
        fprintf(stderr, "Error: Key generation failed.\n");
        EVP_PKEY_CTX_free(kctx);
        return 1;
    }
    EVP_PKEY_CTX_free(kctx);
    printf("    Key generated successfully.\n\n");

    printf("[2] RSA-%d with OAEP padding can encrypt at most %d bytes.\n\n",
           KEY_BITS, MAX_PAYLOAD);


    unsigned char small_msg[100];
    memset(small_msg, 'A', sizeof(small_msg));
    printf("[3] Encrypting small message (%zu bytes)...\n", sizeof(small_msg));

    unsigned char *ct1 = NULL;
    size_t ct1_len = 0;
    if (rsa_encrypt(pkey, small_msg, sizeof(small_msg), &ct1, &ct1_len)) {
        printf("    [OK] Success. Ciphertext size: %zu bytes.\n\n", ct1_len);
        free(ct1);
    } else {
        printf("    [FAIL] Unexpected failure on small message.\n\n");
    }


    unsigned char large_msg[1000];
    memset(large_msg, 'B', sizeof(large_msg));
    printf("[4] Encrypting large message (%zu bytes)...\n", sizeof(large_msg));

    unsigned char *ct2 = NULL;
    size_t ct2_len = 0;
    if (rsa_encrypt(pkey, large_msg, sizeof(large_msg), &ct2, &ct2_len)) {
        printf("    [OK] Success. Ciphertext size: %zu bytes.\n\n", ct2_len);
        free(ct2);
    } else {
        printf("    [ERROR] data too large for key size\n");
        printf("    RSA cannot encrypt %zu bytes directly.\n", sizeof(large_msg));
        printf("    Max allowed with OAEP: %d bytes.\n\n", MAX_PAYLOAD);
    }

    printf("[5] Conclusion:\n");
    printf("    Asymmetric encryption (RSA) is a heavy mathematical operation.\n");
    printf("    It cannot encrypt photos, videos, or large documents directly.\n");
    printf("    Solution: Hybrid encryption (generate a random AES key,\n");
    printf("    encrypt the data with AES, encrypt the AES key with RSA).\n");

    EVP_PKEY_free(pkey);
    return 0;
}
