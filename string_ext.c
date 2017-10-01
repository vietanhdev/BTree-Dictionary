#include <string.h>
#include <ctype.h>


char * strLower(char * dest, char * s) {
  int i;
  for (i = 0; i < strlen(s); i++) {
    dest[i] = tolower(s[i]);
  }
  return dest;
}

char * strUpper(char * dest, char * s) {
  int i;
  for (i = 0; i < strlen(s); i++) {
    dest[i] = toupper(s[i]);
  }
  return dest;
}