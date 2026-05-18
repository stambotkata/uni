#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *secret = "Hello";
    int secret_len = (int)strlen(secret) + 1;

    FILE *src = fopen("image.bmp", "rb");
    if (!src) {
        fprintf(stderr, "Error: Cannot open 'image.bmp'.\n");
        return 1;
    }

    FILE *dst = fopen("output.bmp", "wb");
    if (!dst) {
        fprintf(stderr, "Error: Cannot create 'output.bmp'.\n");
        fclose(src);
        return 1;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, src) != 54) {
        fprintf(stderr, "Error: File is too small or not a valid BMP.\n");
        fclose(src);
        fclose(dst);
        return 1;
    }
    fwrite(header, 1, 54, dst);

    printf("--- LSB Encoder ---\n");
    printf("Hiding message: \"%s\"\n", secret);
    printf("Total bytes to hide (including '\\0'): %d\n\n", secret_len);

    for (int i = 0; i < secret_len; i++) {
        unsigned char ch = (unsigned char)secret[i];

        for (int bit = 0; bit < 8; bit++) {
            unsigned char pixel;

            if (fread(&pixel, 1, 1, src) != 1) {
                fprintf(stderr, "Error: Image is too small to hide the entire message.\n");
                fclose(src);
                fclose(dst);
                return 1;
            }

            unsigned char secret_bit = (ch >> bit) & 1;
            pixel = (pixel & 0xFE) | secret_bit;
            fwrite(&pixel, 1, 1, dst);
        }
    }

    unsigned char buf[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, sizeof(buf), src)) > 0) {
        fwrite(buf, 1, bytes_read, dst);
    }

    fclose(src);
    fclose(dst);

    printf("[SUCCESS] Message hidden in 'output.bmp'.\n");
    printf("Verify with the LSB decoder on output.bmp.\n");

    return 0;
}
