//
// Created by 86180 on 2022/12/12.
//

#ifndef _BOOKSTORE_TRANSACTIONLOG_H
#define _BOOKSTORE_TRANSACTIONLOG_H
#include <fstream>
class BookFile;
class AccountFile;

class TransactionLog{
public:
    TransactionLog();
    ~TransactionLog();
    void cost(double); //negative
    void earn(double); //positive
    void ShowAll() const;
    void Show(int count);
private:
    int transaction_num;
    std::fstream iof;
    double total;
    double earn_total;  //positive
    double cost_total;  //negative
};
#endif //_BOOKSTORE_TRANSACTIONLOG_H
