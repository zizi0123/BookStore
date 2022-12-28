#include <iostream>
#include "transaction_log.h"

TransactionLog::TransactionLog() {
    std::ifstream in("file/transaction_log");
    if (!in) {  //第一次生成此文件
        std::ofstream out("file/transaction_log");
        out.close();
        transaction_num = 0;
        total = 0;
        earn_total = 0;
        cost_total = 0;
        iof.open("file/transaction_log", std::fstream::in | std::fstream::out);
        iof.write(reinterpret_cast<char *>(&transaction_num), sizeof(int));
        iof.write(reinterpret_cast<char *>(&total), sizeof(long long));
        iof.write(reinterpret_cast<char *>(&earn_total), sizeof(long long));
        iof.write(reinterpret_cast<char *>(&cost_total), sizeof(long long));
    } else {
        iof.open("file/transaction_log", std::fstream::in | std::fstream::out);
        iof.read(reinterpret_cast<char *>(&transaction_num), sizeof(int));
        iof.read(reinterpret_cast<char *>(&total), sizeof (long long));
        iof.read(reinterpret_cast<char *>(&earn_total), sizeof(long long));
        iof.read(reinterpret_cast<char *>(&cost_total), sizeof(long long));
    }
    in.close();
}

TransactionLog::~TransactionLog() {
    iof.seekp(0);
    iof.write(reinterpret_cast<char *>(&transaction_num), sizeof(int));
    iof.write(reinterpret_cast<char *>(&total), sizeof(long long));
    iof.write(reinterpret_cast<char *>(&earn_total), sizeof(long long));
    iof.write(reinterpret_cast<char *>(&cost_total), sizeof(long long));
    iof.close();
}

void TransactionLog::cost(long long x) {  //x是一个整数，保留了两位小数。17.234->1723 12->1200
    long long temp = -x;
    transaction_num++;
    iof.seekp(0, std::fstream::end);
    iof.write(reinterpret_cast<char *>(&temp), sizeof(long long));
    cost_total += x;
    total -= x;
}

void TransactionLog::earn(long long x) {  //x>0
    long long temp=x;
    transaction_num++;
    iof.seekp(0, std::fstream::end);
    iof.write(reinterpret_cast<char *>(&temp), sizeof(long long));
    earn_total += x;
    total += x;
}

void TransactionLog::ShowAll() const {
    double real_earn = (double) earn_total / 100.00;
    double real_cost = (double) cost_total / 100.00;
    if(cost_total==0){
        printf("+ %.2f - 0.00\n", real_earn);  //处理-0；
    }else{
        printf("+ %.2f - %.2f\n", real_earn, real_cost);
    }
}

void TransactionLog::Show(int count) {
    long long earn = 0;
    long long cost = 0;
    if (count > transaction_num) {
        std::cout << "Invalid\n";
        return;
    }
    iof.seekg(sizeof(int) + sizeof(long long) * (transaction_num - count + 3));
    for (int i = 1; i <= count; ++i) {
        long long temp;
        iof.read(reinterpret_cast<char *>(&temp), sizeof(long long));
        if (temp > 0) {
            earn += temp;
        } else {
            cost -= temp;
        }
    }

    double real_earn = (double) earn / 100.00;
    double real_cost = (double) cost / 100.00;
    if(cost==0){
        printf("+ %.2f - 0.00\n", real_earn);  //处理-0；
    }else{
        printf("+ %.2f - %.2f\n", real_earn, real_cost);
    }
}
