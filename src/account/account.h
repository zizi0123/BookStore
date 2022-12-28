//
// Created by 86180 on 2022/12/9.
//

#ifndef _BOOKSTORE_ACCOUNT_H
#define _BOOKSTORE_ACCOUNT_H
#include <iostream>
#include <fstream>
#include <cstring>
#include "../urll/urll.h"
#include "../status/status.h"
#include "../log/log.h"

class LogStatus;
class BookFile;
class AccountFile;
class TransactionLog;

struct AccountInfo{
    char UserId[31];
    char name[31];
    char password[31];
    int priority;
    AccountInfo()=default;
    AccountInfo(const char*,const char *,const char*,int );
};
class AccountFile{
public:
    UnrollLink id_num;
    AccountFile();
    ~AccountFile();
    void RegisterAccount(const char *UserId,const char *password,const char *name,TransactionLog&);
    void ChangePassword(const char *UserId, const char *new_password, LogStatus &log_status,TransactionLog&,const char *current_password="");
    void CreatAccount(const char *UserId,const char *password,const int priority,const char *name,LogStatus &,TransactionLog&);
    void DeleteAccount(const char *UserId,LogStatus &,TransactionLog&);
    std::fstream iof;
private:
};

#endif //_BOOKSTORE_ACCOUNT_H
