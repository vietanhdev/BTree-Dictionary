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
//dict_t dictCreate(const char * name, const char * path) {};

// mở từ điển đã tạo. đồng thời tạo luôn wordList . nếu thành công trả về 1. xảy ra lỗi gì thì in ra màn hình và trả về 0
//int dictOpen(dict_t dict) {};

// đóng từ điển
//int dictClose(dict_t dict) {};

// tạo wordList cho từ điển
//void makeWordList(BTA * dict, char *** wordList, int * wordListSize);

// tạo từ điển mới từ file txt
void createDictionaryDBFromText(const char * textFileName, const char * dictFileName, BTA ** dict, GtkTextBuffer  *notifyBuff);


// tra từ
//  trả về gía trị = gía trị btsel
//int dictFindWord(dict_t dict, const char * word, char * meaning);

// tra tu tiep theo.
// sau khi tra tu se thay current word thanh tu tiep theo tra duoc (neu co)
//int dictFindPNextWord(dict_t dict, char * currentWord, char * meaning);

// tra tu truoc do
// sau khi tra tu se thay current word thanh tu truoc do (neu co)
//int dictFindPNextWord(dict_t dict, char * currentWord, char * meaning);


// thực hiện so sánh để chọn suggest trong hàm wordListSuggest();
//int suggestWordCmp(char * str, char * word);

// tạo 1 danh sách các từ gợi ý dựa vào wordList của từ điển
// Trả về số lượng từ trong suggestList
//int wordListSuggest(char ** suggestList, dict_t dict);



// Các hàm sau đây sẽ được viết lại thay thế bằng các hàm trên. Xóa đi khi viết xog
int dictFindWord(BTA *dict, const char * word, char * meaning);
int dictFindNextWord(BTA * dict, char * currentWord, char * meaning);
int dictFindPrevWord(BTA * dict, char * currentWord, char * meaning);

void wordListAddWord(char * word, char *** wordList, int * wordListSize);
void wordListEmpty(char *** wordList, int * wordListSize);
int wordListSuggest(char * suggestStr, char * str, char ** wordList, int wordListSize);


#endif