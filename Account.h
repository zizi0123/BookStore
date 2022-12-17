//
// Created by 86180 on 2022/12/9.
//

#ifndef _BOOKSTORE_ACCOUNT_H
#define _BOOKSTORE_ACCOUNT_H
#include <iostream>
#include <fstream>
struct Account{
    char UserId[31];
    char name[31];
    char password[31];
    int priority;
};
class user_account{
public:
    user_account();
    user_account(const std::string UserId,const std::string name,const std::string password,const int priority);
    ~user_account();
    void Register_account(const std::string UserId,const std::string name,const std::string password,const int priority);
    void ChangePassword(const std::string UserId,const std::string new_password,const std::string current_password="");
    void CreatAccount(const std::string UserId,const std::string name,const std::string password,const int priority);
    void DeleteAccount(const std::string UserId);
};
class user_file{

};
#endif //_BOOKSTORE_ACCOUNT_H
