#include <cstring>
#include <iostream>
#include "bookstore.h"

int main() {
    std::filesystem::create_directory("file");
    freopen("../bookstore-testcases/basic/testcase5.in", "r", stdin);
    freopen("out", "w", stdout);
    BookFile book_file;
    AccountFile account_file;
    LogStatus log_status;
    TransactionLog transaction_log;
    int count = 0;
    while (true) {
        count++;
        char command[1000];
        std::cin.getline(command, 1000);
        if (!std::cin) break;
//        std::cout<<count<<' ';
        command[strlen(command)] = '\0';
        try {
            ProcessLine(book_file, account_file, log_status, transaction_log, command);
        } catch (error) {
            std::cout << "Invalid\n";
        } catch (quit) {
            break;
        }
//        std::cout<<'\n';
    }


}
