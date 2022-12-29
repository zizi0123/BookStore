
#ifndef BOOKKKKKK
#define BOOKKKKKK
#include <iostream>
#include <fstream>
#include <iostream>
#include <set>
#include "../urll/urll.h"
#include "../status/status.h"
#include "../utils/strlib.h"
#include "../log/log.h"
#include "../bookstore.h"

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
    long long price;
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
    void buy(const char *ISBN,const int &quantity,TransactionLog &,LogStatus&,AccountFile&);
    void TryModifyISBN(const char *ISBN,LogStatus& );
    void TryModifyname(const char *name,LogStatus&);
    void TryModifyauthor(const char *author,LogStatus&);
    void TryModifykeyword(const char *keyword,LogStatus&);
    void TryModifyprice( long long price,LogStatus&);
    void ModifyISBN(const char *ISBN,LogStatus& );
    void Modifyname(const char *name,LogStatus&);
    void Modifyauthor(const char *author,LogStatus&);
    void Modifykeyword(const char *keyword,LogStatus&);
    void Modifyprice( long long price,LogStatus&);
    void import(int quantity,long long cost,LogStatus&,TransactionLog &,LogStatus&,AccountFile&);
    UnrollLink isbn_num ;
    UnrollLink bookname_num;
    UnrollLink author_num;
    UnrollLink keyword_num;
    std::fstream iof;
};


#endif //BOOKKKKKK
