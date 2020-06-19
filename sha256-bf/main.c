#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void hex2bin(char* src, char* dst, int dst_size) {
    char *buffer = malloc(sizeof(char) * 3);
    buffer[2] = '\0';
    for (int i = 0; i < dst_size; i++) {
        buffer[0] = src[i * 2];
        buffer[1] = src[i * 2 + 1];
        dst[i] = (char) strtol(buffer, NULL, 16);
    }
    free(buffer);
}

int main(int argc, char** argv) {
    if (argc != 3  || strlen(argv[1]) != 16 || strlen(argv[2]) != 64) {
        return -1;
    } else {
        char* charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        int charset_len = strlen(charset);
        char* known = argv[1];
        char* hexdigest = argv[2];

        char* buffer = malloc(sizeof(char) * 20);
        char* tmp_digest = malloc(sizeof(char) * 32);
        char* digest = malloc(sizeof(char) * 32);

        hex2bin(hexdigest, digest, 32);
        memcpy(buffer + 4, known, 16);

        for (int l1 = 0; l1 < charset_len; l1 ++) {
            buffer[0] = charset[l1];
            for (int l2 = 0 ; l2 < charset_len; l2 ++) {
                buffer[1] = charset[l2];
                for (int l3 = 0; l3 < charset_len; l3 ++) {
                    buffer[2] = charset[l3];
                    for (int l4 = 0; l4 < charset_len; l4 ++) {
                        buffer[3] = charset[l4];
                        SHA256(buffer, 20, tmp_digest);
                        if (memcmp(tmp_digest, digest, 32) == 0) {
                            printf("%s\n", buffer);
                            goto stop;
                        }
                    }
                }
            }
        }
        stop:
        free(buffer);
        free(tmp_digest);
        free(digest);
        return 0;
    }
}