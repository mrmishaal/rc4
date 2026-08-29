#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY "mishaal123"
#define KEY_LEN 14

void rc4(const unsigned char *key, int key_len, unsigned char *data, int data_len) {
    unsigned char S[256];
    int i, j;

    for (i = 0; i < 256; i++) S[i] = i;

    j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key_len]) % 256;
        unsigned char tmp = S[i]; S[i] = S[j]; S[j] = tmp;
    }

    i = 0; j = 0;
    for (int k = 0; k < data_len; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        unsigned char tmp = S[i]; S[i] = S[j]; S[j] = tmp;
        data[k] ^= S[(S[i] + S[j]) % 256];
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    const char *path = argv[1];

    FILE *f = fopen(path, "rb");
    if (!f) { perror("fopen"); return 1; }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buf = malloc(size);
    if (!buf) { perror("malloc"); fclose(f); return 1; }

    fread(buf, 1, size, f);
    fclose(f);

    rc4((const unsigned char *)KEY, KEY_LEN, buf, (int)size);

    f = fopen(path, "wb");
    if (!f) { perror("fopen"); free(buf); return 1; }
    fwrite(buf, 1, size, f);
    fclose(f);

    printf("Encrypted %ld bytes to %s\n", size, path);
    free(buf);
    return 0;
}
