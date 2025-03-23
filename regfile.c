#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"

#define REG_NUM 32

static void phex(uint8_t* str);
static void encrypt_ecb(uint8_t* key, uint8_t* pt);
static void decrypt_ecb(uint8_t* key, uint8_t* ct);

int main() {
    FILE* fp = fopen("regs", "r");
    uint8_t key[AES_BLOCKLEN+1];
    uint8_t regs[REG_NUM][AES_BLOCKLEN+1];
    size_t num_read;

    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    num_read = fread((char*) key, 1, AES_BLOCKLEN, fp);
    key[AES_BLOCKLEN] = '\0';
    if (num_read != AES_BLOCKLEN) {
        printf("Key must be 16 bytes\n");
        return 1;
    }

    for (int i = 0 ; i < REG_NUM ; i++) {
        num_read = fread((char*) regs[i], 1, AES_BLOCKLEN, fp);
        regs[i][AES_BLOCKLEN] = '\0';
        if (num_read != AES_BLOCKLEN) {
            printf("Reg must be 16 bytes\n");
            return 1;
        }
    }

    if (fclose(fp) == EOF) {
        printf("Error closing file\n");
        return 1;
    }

    for (int i = 0 ; i < REG_NUM ; i++) {
        printf("%d: ", i);
        decrypt_ecb(key, regs[i]);
        phex(regs[i]);
    }

    return 0;
}

// print bytes as hex
static void phex(uint8_t* str)
{
    uint8_t len = 16;

    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}

static void encrypt_ecb(uint8_t* key, uint8_t* pt)
{
    struct AES_ctx ctx;

    AES_init_ctx(&ctx, key);
    AES_ECB_encrypt(&ctx, pt);
}

static void decrypt_ecb(uint8_t* key, uint8_t* ct)
{
    struct AES_ctx ctx;

    AES_init_ctx(&ctx, key);
    AES_ECB_decrypt(&ctx, ct);
}