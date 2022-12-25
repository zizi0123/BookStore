
#ifndef BOOKKKKKK
#define BOOKKKKKK
#include <iostream>
#include <fstream>
#include <iostream>
#include <set>
#include "unrolllinklist.h"
#include "log_status.h"
#include "token_scanner.h"
#include "transaction_log.h"

class LogStatus;
class AccountFile;
class TransactionLog;

struct BookInfo{
    char ISBN[21];
    char name[61];
    char author[61];
    char org_keywords [61];
//    std::vector<std::string>keywords;
    int quantity;
    double price;
};

bool operator<(const BookInfo&x,const BookInfo&y);
class BookFile{
public:
    BookFile();
    ~BookFile();
    void show_ISBN(const char *ISBN);
    void show_name(const char *name);
    void show_author(const char *author);
    void show_keyword(const char *keyword);
    void show_all();
//    std::vector<int> isbn_to_num(const char *);
    void buy(const char *ISBN,const int &quantity,TransactionLog &);
    void modify_ISBN(const char *ISBN,LogStatus& );
    void modify_name(const char *name,LogStatus&);
    void modify_author(const char *author,LogStatus&);
    void modify_keyword(const char *keyword,LogStatus&);
    void modify_price(double prize,LogStatus&);
    void import(int quantity,double cost,LogStatus&,TransactionLog &);
    UnrollLink isbn_num ;
    UnrollLink bookname_num;
    UnrollLink author_num;
    UnrollLink keyword_num;
    std::fstream iof;   //读第num个位置的信息BookInfo
    int book_total_num; //书籍的总量
private:
};


#endif //BOOKKKKKK
