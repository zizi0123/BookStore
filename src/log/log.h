#ifndef _BOOKSTORE_TRANSACTIONLOG_H
#define _BOOKSTORE_TRANSACTIONLOG_H
#include <fstream>
#include <cstring>
#include "../status/status.h"
class BookFile;
class AccountFile;
class LogStatus;

struct Record{
    char command [500]="";
    char UserId[31]="";
    int pri=-1;
    Record()=default;
    Record(const char* command ,const char* UserId,const int &pri);
    explicit Record(const char* command);
};

class TransactionLog{
public:
    TransactionLog();
    ~TransactionLog();
    void cost(long long); //negative
    void earn(long long); //positive
    void ShowAll() const;
    void Show(int count);
    void AddRecord(const char* command ,const char* UserId,int pri);
    void AddRecord(const char* command);
    void log(LogStatus&);
private:
    int transaction_num;
    int record_num;
    std::fstream iof;
    std::fstream iof2;
    long long total;
    long long earn_total;  //positive
    long long cost_total;  //positive
};


#endif //_BOOKSTORE_TRANSACTIONLOG_H
