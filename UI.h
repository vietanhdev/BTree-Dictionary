#include "btree.h"
#include "extio.h"
#ifndef UI_MENU_NOTIFY_LENGTH
#define UI_MENU_NOTIFY_LENGTH 200
#endif


#define MENUOPT 3
static const char menuShortcuts[MENUOPT][5] = {
    "a","d","l"
};
static const char menu[MENUOPT][60] = {
    "Add a word",
    "Delete a word",
    "Load from txt file (Only supported format)"
};

static const char InfoBoard[5][400] = {
    "╔════════╤════════════════════════════════════════════════════════════════════╗",
    "║    \033[38;5;228mF                         FOCUS - A DICTIONARY IMPLEMENTED USING BTREE\033[0m   ║",
    "╚════════╧════════════════════════════════════════════════════════════════════╝",
    "                                             Group: PhuongAnh, VietAnh, TienDat",
    "                                               Press ] for menu && ESC to quit."
};

// PROTOTYPES
void UI_init(BTA * g_dict, char * meaningArea, char * g_notify1, char * g_notify2);
void UI_InfoBoard();
void UI_Menu();
void UI_Search(char* searchBox);
void UI_Notify();
void UI_Notify1_Push(char * s);
void UI_Notify2_Push(char * s);
void gotoxy(int x, int y);


