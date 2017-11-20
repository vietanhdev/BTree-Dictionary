#ifndef STRING_EXT_H
#define STRING_EXT_H

#include <gtk/gtk.h>

char * strLower(char * dest, const char * s);
char * strUpper(char * dest, const char * s);
void trim(char * s);

// Function to make valid a utf-8 string
gchar *
g_utf8_make_valid_custom (const gchar *str,
                   gssize       len);


#endif