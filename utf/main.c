#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

char* read_file(char *filename) {
    FILE *fd = fopen(filename, "r");

    fseek(fd, 0, SEEK_END);
    long sz = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char *content = malloc(sizeof(char) * sz);
    fread(content, sizeof(char), sz, fd);

    fclose(fd);
    return content;
}

int utf8_strlen(char *s) {
    int length = 0;

    while (*s) {
        if ((*s & 0b11000000) != 0b10000000) {
            length += 1;
        }
        s++;
    }
    return length;
}

char* codepoint_to_utf8(unsigned int codepoint, int *sz) {
    char *s;
    if (codepoint < 0x80) {
        *sz = 1;
        s = malloc(sizeof(char));
        *s = codepoint;
    } else if (codepoint < 0x800) {
        *sz = 2;
        s = malloc(sizeof(char) * 2);
        *(s + 1) = 0b10000000 | (codepoint & 0b00111111);
        *s = 0b11000000 | ((codepoint >> 6) & 0b00011111);
    } else if (codepoint < 0x10000) {
        *sz = 3;
        s = malloc(sizeof(char) * 3);
        *(s + 2) = 0b10000000 | (codepoint & 0b00111111);
        *(s + 1) = 0b10000000 | ((codepoint >> 6) & 0b00111111);
        *s = 0b11100000 | ((codepoint >> 12) & 0b00001111);
    } else {
        *sz = 4;
        s = malloc(sizeof(char) * 4);
        *(s + 3) = 0b10000000 | (codepoint & 0b00111111);
        *(s + 2) = 0b10000000 | ((codepoint >> 6) & 0b00111111);
        *(s + 1) = 0b10000000 | ((codepoint >> 12) & 0b00111111);
        *s = 0b11110000 | ((codepoint >> 18) & 0b00000111);
    }
    return s;
}

int get_tail_length(char c) {
    int length = 0;
    if ((c & 0b11111000) == 0b11110000) {
        length = 3;
    } else if ((c & 0b11110000) == 0b11100000) {
        length = 2;
    } else if ((c & 0b11100000) == 0b11000000) {
        length = 1;
    }
    return length;
}

char* utf8_substr(char* s, int start, int length) {
    int curr_pos = 0;
    int byte_count = 0;
    int offset = 0;
    char *new_s;

    while (curr_pos < start && *s) {
        if ((*s & 0b11000000) != 0b10000000) {
            curr_pos += 1;
        }
        s += (get_tail_length(*s) + 1);
    }

    curr_pos = 0;
    while (curr_pos < length && *s) {
        if ((*s & 0b11000000) != 0b10000000) {
            curr_pos += 1;
        }
        s++;
        byte_count++;
    }

    if (curr_pos > 0) {
        offset = get_tail_length(*(s - 1));

        new_s = malloc(sizeof(char) * (byte_count + offset + 1));
        memcpy(new_s, s - byte_count, byte_count + offset);
        new_s[byte_count + offset] = '\0';
    } else {
        new_s = NULL;
    }
    return new_s;
}

int utf8_to_codepoint(char *s) {
    int length = 0;
    int codepoint = 0;

    if ((*s & 0b11111000) == 0b11110000) {
        length = 4;
        codepoint = *s & 0b111;
    } else if ((*s & 0b11110000) == 0b11100000) {
        length = 3;
        codepoint = *s & 0b1111;
    } else if ((*s & 0b11100000) == 0b11000000) {
        length = 2;
        codepoint = *s & 0b11111;
    } else {
        length = 1;
        codepoint = *s;
    }
    for (int i = 1; i < length; i++) {
        s++;
        codepoint <<= 6;
        codepoint = codepoint | (*s & 0b00111111);
    }
    return codepoint;
}

int main() {
    /*
    char *content = read_file("/home/rmb122/temp/example.txt");
    printf("%d", utf8_strlen(content));

    free(content);
     */

    printf("%d\n", utf8_strlen("一二三四五六一二三四五六1234"));

    char *s;
    int sz;

    s = codepoint_to_utf8(21834, &sz);
    printf("%.*s\n", sz, s);
    free(s);

    s = utf8_substr("一2三四5六7八9", 1, 1);
    if (s) {
        printf("%s\n", s);
    } else {
        printf("%s", "null");
    }
    free(s);

    printf("%d\n", utf8_to_codepoint("五"));
    return 0;
}
