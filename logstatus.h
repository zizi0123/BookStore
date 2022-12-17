//
// Created by 86180 on 2022/12/9.
//

#ifndef _BOOKSTORE_LOGSTATUS_H
#define _BOOKSTORE_LOGSTATUS_H

#include <string>
#include <vector>
struct info{
    int priority;
    std::string UserId;
};
class log_status{
public:
    log_status();
    ~log_status();
    void SwitchUser(std::string UserId,std::string password="");
    void logout();//撤销最后一次成功执行的 su 指令效果
    void Select(std::string ISBN);
private:
    std::vector<info> user;
    std::vector<std::string> chosen_book;
};
#endif //_BOOKSTORE_LOGSTATUS_H
