#include <stdio.h>

#define clear() printf("\033[H\033[J")

#define RED 31
#define GREEN 32
#define ORANGE 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define YELLOW 38
#define NORMAL 0

void printColor(const char * str, int color);
void cleanStdin(void);
void removeLastEnter(char *s);
char* readLn(FILE * fin, char *s, int max_len);
int getch();
void printUpper(char * s);
void gotoxy(int x,int y);