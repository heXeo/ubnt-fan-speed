#include <stdio.h>
#include <unistd.h>

int file_write_uint(const char* const path, const unsigned int value) {
  FILE* fp = fopen(path, "w");
  if (fp == NULL) {
    return -1;
  }

  int ret = fprintf(fp, "%u", value);
  fclose(fp);

  return ret;
}

int file_read_uint(const char* const path, unsigned int* const value) {
  char buffer[256];
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    return -1;
  }

  char* ptr = fgets(buffer, 256, fp);
  fclose(fp);
  if (ptr == NULL) {
    return -1;
  }
  
  int read = sscanf(buffer, "%u", value);
  if (read == EOF) {
    return -1;
  }
  
  return read;
}

int file_read_int(const char* const path, int* const value) {
  char buffer[256];
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    return -1;
  }

  char* ptr = fgets(buffer, 256, fp);
  fclose(fp);
  if (ptr == NULL) {
    return -1;
  }
  
  int read = sscanf(buffer, "%d", value);
  if (read == EOF) {
    return -1;
  }
  
  return read;
}

int file_exists(const char* const path) {
  return access(path, F_OK);
}
