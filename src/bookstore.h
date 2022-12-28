
#ifndef _BOOKSTORE_BOOKSTORE_H
#define _BOOKSTORE_BOOKSTORE_H

#include <iostream>
#include "utils/token_scanner.h"
#include "status/status.h"
#include "account/account.h"
#include "book/book.h"
#include "log/log.h"
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


void su(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, AccountFile &account_file,TransactionLog &transaction_log);

void RegisterUser(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file,TransactionLog &transaction_log);

void passwd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status,TransactionLog &transaction_log);

void
useradd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status,TransactionLog &transaction_log);

void erase(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status,TransactionLog &transaction_log);

void ShowFinance(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, TransactionLog &, AccountFile &account_file);

void show(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, BookFile &book_file,TransactionLog &transaction_log, AccountFile &account_file);

void modify(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
            BookFile &book_file,TransactionLog &transaction_log, AccountFile &account_file,const char *origin_cmd);

void import(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
            BookFile &book_file, TransactionLog &,AccountFile&);

void select(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
            BookFile &book_file);

void buy(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,
         BookFile &book_file, TransactionLog &,AccountFile&);

#endif //_BOOKSTORE_BOOKSTORE_H
