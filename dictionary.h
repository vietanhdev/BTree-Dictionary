#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <gtk/gtk.h>

#define WORD_MAX_LEN 200
#define MEAN_MAX_LEN 40000
#define LINE_MAX_LEN 40000

#define WORD_WC_MAX_LEN 200 // for widechar type
#define MEAN_WC_MAX_LEN 40000 // for widechar type
#define LINE_WC_MAX_LEN 40000

#define SUGGEST_BOX_MAX_LEN 10000
#define SUGGEST_WORD_NUM 5

#define DICTNAME_MAX_LEN 200
#define DICTPATH_MAX_LEN 500

#ifndef ZKYLEN
#define ZKYLEN WORD_MAX_LEN
#endif

#include "btree.h"

extern char ** dictWordList;
extern int dictWordListSize;


typedef struct dict_struct {
	char name[DICTNAME_MAX_LEN];
	char path[DICTPATH_MAX_LEN];
	BTA * dict;
	char ** wordList;
	int wordListSize;
} dict_t;


// New functions: Các function bên dưới sẽ viết lại

// Tạo từ điển mới (trả về kiểu dict type);
// - Tạo file với thông tin là filename và gán vào BTA * dict (btcrt);
// - Tạo wordList có số phần tử là 0 (chỉ malloc cho wordList **)
dict_t dictCreate(const char * name, const char * path);

// mở từ điển đã tạo. đồng thời tạo luôn wordList . nếu thành công trả về 1. xảy ra lỗi gì thì in ra màn hình và trả về 0
int dictOpen(dict_t * dict);

// đóng từ điển
int dictClose(dict_t * dict);

// add a word to word list
void wordListAddWord(char * word, char *** wordList, int * wordListSize);

// tạo wordList cho từ điển
void wordListBuild(BTA * dict, char *** wordList, int * wordListSize);

// empty a wordList
void wordListEmpty(char *** wordList, int * wordListSize);

// tạo từ điển mới từ file txt
//dict_t text2dict(const char * textFileName, const char * dictFileName, GtkTextBuffer  *notifyBuff);
void createDictionaryDBFromText(dict_t * dict, const char * dictName, const char * textFileName, const char * dbFileName, GtkTextBuffer  *notifyBuff);


// tra từ
//  trả về gía trị = gía trị btsel
int dictFindWord(dict_t dict, const char * word, char * meaning);

// tra tu tiep theo.
// sau khi tra tu se thay current word thanh tu tiep theo tra duoc (neu co)
int dictFindNextWord(dict_t dict, char * currentWord, char * meaning);

// tra tu truoc do
// sau khi tra tu se thay current word thanh tu truoc do (neu co)
int dictFindPrevWord(dict_t dict, char * currentWord, char * meaning);


// thực hiện so sánh để chọn suggest trong hàm wordListSuggest();
int suggestWordCmp(char * str, char * word);

// Give suggestion based on "str"
int wordListSuggest(char * str, char ** suggestList, int suggestListMaxSize, dict_t dict);

// add word to dictionary
int dictAddWord(BTA * dict, char * word, char * meaning, GtkTextBuffer  *notifyBuff);


// delete word from dictionary
void dictDelWord(BTA * dict, char * word, GtkTextBuffer  *notifyBuff);

#endif