#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include "extio.h"

void printColor(const char * str, int color) {
  switch (color) {
    case RED: printf("\x1b[31m"); printf("%s",str); printf("\033[0m"); break;
    case GREEN: printf("\x1b[32m"); printf("%s",str); printf("\033[0m"); break;
    case ORANGE: printf("\x1b[33m"); printf("%s",str); printf("\033[0m"); break;
    case BLUE: printf("\x1b[34m"); printf("%s",str); printf("\033[0m"); break;
    case MAGENTA: printf("\x1b[35m"); printf("%s",str); printf("\033[0m"); break;
    case CYAN: printf("\x1b[35m"); printf("%s",str); printf("\033[0m"); break;
    case YELLOW: printf("\033[38;5;228m"); printf("%s",str); printf("\033[0m"); break;
    default: printf("%s", str);
  }
}

void cleanStdin(void) {
	int c;
	do { c = getchar(); } while (c != '\n' && c != EOF);
}

void removeLastEnter(char *s) {
	if (strlen(s) != 0)
		if (s[strlen(s) - 1] == '\n')
			s[strlen(s) - 1] = '\0';
}

char* readLn(FILE * fin, char *s, int max_len) {
	char* p;
	p = fgets(s, max_len * sizeof(char), fin);
	
	if (p != NULL) { // already read sth
		// Re-read line if it contain only enter character
		removeLastEnter(s);
		if (strlen(s) == 0) {
			p = readLn(fin, s, max_len);
		}
	}

	return p;
}

// Function to read input keys from keyboard
int getch() {
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

void printUpper(char * s) {
  int i;
  for (i = 0; i < strlen(s); ++i) {
    printf("%c", toupper(s[i]));
  }
}

