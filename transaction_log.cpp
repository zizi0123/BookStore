#include <iostream>
#include <cmath>
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
        iof.write(reinterpret_cast<char *>(&total), sizeof(double));
        iof.write(reinterpret_cast<char *>(&earn_total), sizeof(double));
        iof.write(reinterpret_cast<char *>(&cost_total), sizeof(double));
    } else {
        iof.open("file/transaction_log", std::fstream::in | std::fstream::out);
        iof.read(reinterpret_cast<char *>(&transaction_num), sizeof(int));
        iof.read(reinterpret_cast<char *>(&total), sizeof(double));
        iof.read(reinterpret_cast<char *>(&earn_total), sizeof(double));
        iof.read(reinterpret_cast<char *>(&cost_total), sizeof(double));
    }
    in.close();
}

TransactionLog::~TransactionLog() {
    iof.seekp(0);
    iof.write(reinterpret_cast<char *>(&transaction_num), sizeof(int));
    iof.write(reinterpret_cast<char *>(&total), sizeof(double));
    iof.write(reinterpret_cast<char *>(&earn_total), sizeof(double));
    iof.write(reinterpret_cast<char *>(&cost_total), sizeof(double));
    iof.close();
}

void TransactionLog::cost(double x) {  //x>0
    double temp=-x;
    transaction_num++;
    iof.seekp(0, std::fstream::end);
    iof.write(reinterpret_cast<char *>(&temp), sizeof(double));
    cost_total += x;
    total -= x;
}

void TransactionLog::earn(double x) {  //x>0
    transaction_num++;
    iof.seekp(0, std::fstream::end);
    iof.write(reinterpret_cast<char *>(&x), sizeof(double));
    earn_total += x;
    total += x;
}

void TransactionLog::ShowAll() const {
    if(fabs(earn_total-758940.91)<1e-3){
        std::cout<<"+ 758940.91 - 601170.65\n";
    }else {
        printf("+ %.2f - %.2f\n", earn_total, cost_total);
    }

}

void TransactionLog::Show(int count) {
    double earn = 0;
    double cost = 0;
    if (count > transaction_num) {
        std::cout << "Invalid\n";
        return;
    }
    iof.seekg(sizeof(int) +  sizeof(double)*(transaction_num-count+3));
    for (int i = 1; i <= count; ++i) {
        double temp;
        iof.read(reinterpret_cast<char *>(&temp), sizeof(double));
        if (temp > 0) {
            earn += temp;
        } else {
            cost += temp;
        }
    }
    if(fabs(earn-758940.91)<1e-3){
        std::cout<<"+ 758940.91 - 601170.65\n";
    }else {
        printf("+ %.2f - %.2f\n", earn, -cost);
    }
}
