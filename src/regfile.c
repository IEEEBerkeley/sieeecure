#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "aes-c/aes.h"

#define REG_NUM 32
#define REGFILE_NAME_SIZE 256
#define POLL_INTERVAL 1

static void phex(uint8_t *str);
static void encrypt_ecb(uint8_t *key, uint8_t *pt);
static void decrypt_ecb(uint8_t *key, uint8_t *ct);
static int handle_regs();
static int get_regfile_path(char regfile[], size_t size);

static char REGFILE[PATH_MAX];
static char REGFILE_NAME[REGFILE_NAME_SIZE] = "regs";
int main() {

  get_regfile_path(REGFILE, PATH_MAX);

  struct stat file_stat;
  time_t last_mtime = 0;

  if (stat(REGFILE, &file_stat) < 0) {
    perror("Initializing stat error");
    return 1;
  }

  last_mtime = file_stat.st_mtime;

  if (handle_regs()) {
    perror("Error with handling regfile before polling");
    return 1;
  }

  while (1) {
    sleep(POLL_INTERVAL);

    if (access(REGFILE, F_OK)) {
      perror("File access error");
      continue;
    }

    if (stat(REGFILE, &file_stat) < 0) {
      perror("Error with stat");
      continue;
    }

    if (file_stat.st_mtime != last_mtime) {
      last_mtime = file_stat.st_mtime;
      if (handle_regs()) {
        perror("Error with handling regfile");
        return 1;
      }
    }
  }

  return 0;
}

static int get_regfile_path(char regfile[], size_t size) {
  size_t len;
  if (getcwd(regfile, size) == NULL) {
    perror("Failed to get cwd");
    return 1;
  }
  len = strnlen(regfile, size);
  regfile[len] = '/';
  strncpy(regfile + len + 1, REGFILE_NAME,
          strnlen(REGFILE_NAME, REGFILE_NAME_SIZE));
  len = strnlen(regfile, len + 1 + REGFILE_NAME_SIZE);
  regfile[len] = '\0';
  return 0;
}

static int handle_regs() {
  FILE *fp = fopen("regs", "r");
  uint8_t key[AES_BLOCKLEN + 1];
  uint8_t regs[REG_NUM][AES_BLOCKLEN + 1];
  size_t num_read;

  if (fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  num_read = fread((char *)key, 1, AES_BLOCKLEN, fp);
  key[AES_BLOCKLEN] = '\0';
  if (num_read != AES_BLOCKLEN) {
    perror("Key must be 16 bytes");
    return 1;
  }

  for (int i = 0; i < REG_NUM; i++) {
    num_read = fread((char *)regs[i], 1, AES_BLOCKLEN, fp);
    regs[i][AES_BLOCKLEN] = '\0';
    if (num_read != AES_BLOCKLEN) {
      perror("Reg must be 16 bytes");
      return 1;
    }
  }

  if (fclose(fp) == EOF) {
    perror("Error closing file");
    return 1;
  }

  for (int i = 0; i < REG_NUM; i++) {
    printf("%d: ", i);
    decrypt_ecb(key, regs[i]);
    phex(regs[i]);
  }

  printf("\n");

  return 0;
}

// print bytes as hex
static void phex(uint8_t *str) {
  uint8_t len = 16;

  unsigned char i;
  for (i = 0; i < len; ++i)
    printf("%.2x", str[i]);
  printf("\n");
}

static void encrypt_ecb(uint8_t *key, uint8_t *pt) {
  struct AES_ctx ctx;

  AES_init_ctx(&ctx, key);
  AES_ECB_encrypt(&ctx, pt);
}

static void decrypt_ecb(uint8_t *key, uint8_t *ct) {
  struct AES_ctx ctx;

  AES_init_ctx(&ctx, key);
  AES_ECB_decrypt(&ctx, ct);
}
