#include <iostream>
#include "tokenscanner.h"
#include "logstatus.h"
#include "Account.h"
#include "book.h"
#include "transactionlog.h"
class error;
void ProcessLine(book_file &,user_file &, log_status &, transaction_log &);
int main() {
    book_file _book_file;
    user_file _user_file;
    log_status _log_status;
    transaction_log _transaction_log;
    while(true){
        std::string command;
        std::getline(std::cin,command);
        if(command.empty()) exit(0);
        try{
            ProcessLine(_book_file,_user_file,_log_status,_transaction_log);
        }catch (error){
            std::cout<<"Invalid\n";
        }
    }
}
void ProcessLine(book_file &bookfile,user_file &userfile, log_status &logstatus, transaction_log &transactionlog){

}
class error{

};