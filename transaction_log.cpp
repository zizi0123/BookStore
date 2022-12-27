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
        iof.write(reinterpret_cast<char *>(&total), sizeof(int));
        iof.write(reinterpret_cast<char *>(&earn_total), sizeof(int));
        iof.write(reinterpret_cast<char *>(&cost_total), sizeof(int));
    } else {
        iof.open("file/transaction_log", std::fstream::in | std::fstream::out);
        iof.read(reinterpret_cast<char *>(&transaction_num), sizeof(int));
        iof.read(reinterpret_cast<char *>(&total), sizeof(int));
        iof.read(reinterpret_cast<char *>(&earn_total), sizeof(int));
        iof.read(reinterpret_cast<char *>(&cost_total), sizeof(int));
    }
    in.close();
}

TransactionLog::~TransactionLog() {
    iof.seekp(0);
    iof.write(reinterpret_cast<char *>(&transaction_num), sizeof(int));
    iof.write(reinterpret_cast<char *>(&total), sizeof(int));
    iof.write(reinterpret_cast<char *>(&earn_total), sizeof(int));
    iof.write(reinterpret_cast<char *>(&cost_total), sizeof(int));
    iof.close();
}

void TransactionLog::cost(int x) {  //x是一个整数，保留了两位小数。17.234->1723 12->1200
    int temp = -x;
    transaction_num++;
    iof.seekp(0, std::fstream::end);
    iof.write(reinterpret_cast<char *>(&temp), sizeof(int));
    cost_total += x;
    total -= x;
}

void TransactionLog::earn(int x) {  //x>0
    transaction_num++;
    iof.seekp(0, std::fstream::end);
    iof.write(reinterpret_cast<char *>(&x), sizeof(int));
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
    int earn = 0;
    int cost = 0;
    if (count > transaction_num) {
        std::cout << "Invalid\n";
        return;
    }
    iof.seekg(sizeof(int) + sizeof(int) * (transaction_num - count + 3));
    for (int i = 1; i <= count; ++i) {
        int temp;
        iof.read(reinterpret_cast<char *>(&temp), sizeof(int));
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
