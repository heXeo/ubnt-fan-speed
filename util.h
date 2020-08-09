#ifndef _UTIL_H_
#define _UTIL_H_

  int file_write_uint(const char* const path, unsigned int value);
  int file_read_uint(const char* const path, unsigned int* const value);
  int file_read_int(const char* const path, int* const value);
  int file_exists(const char* const path);

#endif
