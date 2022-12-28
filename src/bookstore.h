
#ifndef _BOOKSTORE_BOOKSTORE_H
#define _BOOKSTORE_BOOKSTORE_H



#include <iostream>
#include "token_scanner.h"
#include "log_status.h"
#include "account.h"
#include "book.h"
#include "transaction_log.h"
#include <string>
#include <filesystem>
#include <valarray>

class error : public std::exception {

};

class quit : public std::exception {

};

class LogStatus;

void ProcessLine(BookFile &book_file, AccountFile &account_file, LogStatus &log_status, TransactionLog &transaction_log,
                 const char *command);

inline bool LetterNum_(const char &x); //判断是否是只有字母或数字或下划线  UserId,Password

inline bool Visible(const char &x); //判断是都均为可见ASCII字符  Username


void su(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, AccountFile &account_file);

void RegisterUser(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file);

void passwd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status);

void
useradd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status);

void erase(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status);

void ShowFinance(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, TransactionLog &);

void show(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, BookFile &book_file);

void modify(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
            BookFile &book_file);

void import(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
            BookFile &book_file, TransactionLog &);

void select(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
            BookFile &book_file);

void buy(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
         BookFile &book_file, TransactionLog &);

#endif //_BOOKSTORE_BOOKSTORE_H
