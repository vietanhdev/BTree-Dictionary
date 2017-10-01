#include <string.h>
#include <ctype.h>
#include "extio.h"

char * strLower(char * dest, char * s) {
  int i;
  for (i = 0; i < strlen(s); i++) {
    dest[i] = tolower(s[i]);
  }
  dest[strlen(s)] = '\0';
  return dest;
}

char * strUpper(char * dest, char * s) {
  int i;
  for (i = 0; i < strlen(s); i++) {
    dest[i] = toupper(s[i]);
  }
  dest[strlen(s)] = '\0';
  return dest;
}