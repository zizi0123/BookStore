//
// Created by 86180 on 2022/12/9.
//

#ifndef _BOOKSTORE_ACCOUNT_H
#define _BOOKSTORE_ACCOUNT_H
#include <iostream>
#include <fstream>
#include "unrolllinklist.h"
#include "log_status.h"

class LogStatus;
class BookFile;
class AccountFile;

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
    void RegisterAccount(const char *UserId,const char *password,const char *name);
    void ChangePassword(const char *UserId, const char *new_password, LogStatus &log_status,const char *current_password="");
    void CreatAccount(const char *UserId,const char *password,const int priority,const char *name,LogStatus &);
    void DeleteAccount(const char *UserId,LogStatus &);
    std::fstream iof;
private:
};

#endif //_BOOKSTORE_ACCOUNT_H
