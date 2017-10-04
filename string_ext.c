#include <string.h>
#include <ctype.h>


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


void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}   