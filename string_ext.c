#include <gtk/gtk.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char * strLower(char * dest, const char * s) {
  int i;
  for (i = 0; i < strlen(s); i++) {
    dest[i] = tolower(s[i]);
  }
  dest[strlen(s)] = '\0';
  return dest;
}

char * strUpper(char * dest, const char * s) {
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

// Function to make valid a utf-8 string
gchar *
g_utf8_make_valid_custom (const gchar *str,
                   gssize       len)
{
  GString *string;
  const gchar *remainder, *invalid;
  gsize remaining_bytes, valid_bytes;

  g_return_val_if_fail (str != NULL, NULL);

  if (len < 0)
    len = strlen (str);

  string = NULL;
  remainder = str;
  remaining_bytes = len;

  while (remaining_bytes != 0) 
    {
      if (g_utf8_validate (remainder, remaining_bytes, &invalid)) 
	break;
      valid_bytes = invalid - remainder;
    
      if (string == NULL) 
	string = g_string_sized_new (remaining_bytes);

      g_string_append_len (string, remainder, valid_bytes);
      /* append U+FFFD REPLACEMENT CHARACTER */
      //g_string_append (string, "\357\277\275");
      g_string_append (string, " ");
      
      remaining_bytes -= valid_bytes + 1;
      remainder = invalid + 1;
    }
  
  if (string == NULL)
    return g_strndup (str, len);
  
  g_string_append_len (string, remainder, remaining_bytes);
  g_string_append_c (string, '\0');

  g_assert (g_utf8_validate (string->str, -1, NULL));

  return g_string_free (string, FALSE);
}
