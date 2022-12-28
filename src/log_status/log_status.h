//
// Created by 86180 on 2022/12/9.
//

#ifndef _BOOKSTORE_LOGSTATUS_H
#define _BOOKSTORE_LOGSTATUS_H

#include <string>
#include <vector>
#include "book.h"
#include "account.h"

class BookFile;
class AccountFile;

struct LogInfo{
    int priority;
    int booknum;
    int usernum;
};

class LogStatus{
public:
    LogStatus()=default;
    ~LogStatus()=default;
    void SwitchUser(const char *UserId,AccountFile& account_file,const char *password="");
    void logout();//撤销最后一次成功执行的 su 指令效果
    void Select(const char *ISBN,BookFile& book_file);  //让栈尾的用户选择图书
    std::vector<LogInfo> login;

};


#endif //_BOOKSTORE_LOGSTATUS_H
