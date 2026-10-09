#include "string.h"


size_t strlen(const char *str) {
  size_t i = 0;
  while (str[i] != '\0') {
    i++;
  }
  return i;
}

char* strcpy(char* dest, const char* src) {
  for (size_t i = 0; src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  return dest;
}
