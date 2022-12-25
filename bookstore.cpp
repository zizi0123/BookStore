#include <iostream>
#include "token_scanner.h"
#include "log_status.h"
#include "account.h"
#include "book.h"
#include "transaction_log.h"
#include <string>
#include <filesystem>

class error : public std::exception {

};
class quit:public std::exception{

};

void ProcessLine(BookFile &book_file, AccountFile &account_file, LogStatus &log_status, TransactionLog &transaction_log,
                 const char *command);

inline bool LetterNum_(const char &x); //判断是否是只有字母或数字或下划线  UserId,Password

inline bool Visible(const char &x); //判断是都均为可见ASCII字符  Username

void su(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, AccountFile &account_file);

void RegisterUser(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file);

void passwd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status);

void useradd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status);

void erase(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status);

void ShowFinance(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, TransactionLog &);

void show(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,BookFile &book_file);

void modify(const int &num, const std::vector<std::string> &instruct,  LogStatus &log_status,
            BookFile &book_file);

void import(const int &num, const std::vector<std::string> &instruct,  LogStatus &log_status,
            BookFile &book_file,TransactionLog&);

void select (const int &num, const std::vector<std::string> &instruct,  LogStatus &log_status,
             BookFile &book_file);
void buy(const int &num, const std::vector<std::string> &instruct,  LogStatus &log_status,
         BookFile &book_file,TransactionLog &);

int main() {
    std::filesystem::create_directory("file");
//    freopen("../bookstore-testcases/basic/testcase8/2.in","r",stdin);
//    freopen("out","w",stdout);
    BookFile book_file;
    AccountFile account_file;
    LogStatus log_status;
    TransactionLog transaction_log;
    int count=0;
    while (true) {
        count++;
        char command[1000];
        std::cin.getline(command, 1000);
        if(!std::cin) break;
//        std::cout<<count<<' ';
        command[strlen(command)] = '\0';
        try {
            ProcessLine(book_file, account_file, log_status, transaction_log, command);
        } catch (error) {
            std::cout << "Invalid\n";
        } catch (quit){
            break;
        }
//        std::cout<<'\n';
    }



}





void ProcessLine(BookFile &book_file, AccountFile &account_file, LogStatus &log_status, TransactionLog &transaction_log,
                 const char *command) {
    int start=0, end=0;
    int num = 0;  //切片出的指令数
    std::vector<std::string> instruct;
    bool flag_space= true;
    for(int i=0;i<strlen(command);++i){
        if(command[i]!=' ') {
            flag_space= false;
            break;
        }
    }
    if(flag_space) return;  //全是空格，不输出信息
    for (int i = 0; i < strlen(command); ++i) {
        if (!(32 <= int(command[i]) && int(command[i]) <= 126)) {  //指令合法字符集为可见字符或空格（32）
            throw error();
        }
        if (i == 0 && (command[0] != ' ')) {
            start = 0;
        } else if (command[i] == ' ' && command[i + 1] != ' ') {
            start = i + 1;
        }
        if (command[i] != ' ' && (command[i + 1] == ' ' || i == strlen(command) - 1)) {
            ++num;
            end = i;
            std::string temp(command + start, command + end + 1);
            instruct.push_back(temp);
        }
    }
    if (instruct[0] == "quit") {
        if (num != 1) throw error();
        throw quit();
    } else if (instruct[0] == "exit") {
        if (num != 1) throw error();
        throw quit();
    } else if (instruct[0] == "su") {
        su(num, instruct, log_status, account_file);
    } else if (instruct[0] == "logout") {
        if (num != 1) throw error();
        log_status.logout();
    } else if (instruct[0] == "register") {
        RegisterUser(num, instruct, account_file);
    } else if (instruct[0] == "passwd") {
        passwd(num, instruct, account_file, log_status);
    } else if (instruct[0] == "useradd") {
        useradd(num, instruct, account_file, log_status);
    } else if (instruct[0] == "delete") {
        erase(num, instruct, account_file, log_status);
    } else if (instruct[0] == "show") {
        if (instruct.size()>1 && instruct[1] == "finance") {
            ShowFinance(num, instruct, log_status, transaction_log);
        } else {
            show(num, instruct,  log_status, book_file);
        }
    } else if (instruct[0] == "buy") {
        buy(num,instruct,log_status,book_file,transaction_log);
    } else if (instruct[0] == "select") {
        select(num,instruct,log_status,book_file);
    } else if (instruct[0] == "modify") {
        modify(num,instruct,log_status,book_file);
    } else if (instruct[0] == "import") {
        import(num,instruct,log_status,book_file,transaction_log);
    } else if (instruct[0] == "log") {
   //todo log
    } else {
        throw error();
    }
}


inline bool LetterNum_(const char &x) {  //判断是否是只有字母或数字或下划线  UserId,Password
    if ((int(x) >= 48 && int(x) <= 57) || (65 <= int(x) && int(x) <= 90) || (97 <= int(x) && int(x) <= 122) ||
        int(x) == 95)
        return true;
    return false;
}

inline bool Visible(const char &x) { //判断是都均为可见ASCII字符
    if (33 <= int(x) && int(x) <= 126) return true;
    return false;
}

void su(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, AccountFile &account_file) {
    if (num != 3) throw error(); //只可能有三段
    if (instruct[1].size() > 30) throw error();  //最大长度30
    for (const char &i: instruct[1]) {
        if (!LetterNum_(i)) throw error();  //字母数字下划线是合法字符集
    }
    if (num == 2) {
        log_status.SwitchUser(instruct[1].c_str(), account_file);
    } else {
        if (instruct[2].size() > 30) throw error();  //最大长度30
        for (const char &i: instruct[2]) {
            if (!LetterNum_(i))throw error();  //字母数字下划线是合法字符集
        }
        log_status.SwitchUser(instruct[1].c_str(), account_file, instruct[2].c_str());
    }
}

void RegisterUser(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file) {
    if (num != 4) throw error();
    for (int i = 1; i<=2; ++i) {
        if (instruct[i].size() > 30) throw error();
        for (char j : instruct[i]) {
            if (!LetterNum_(j)) throw error();  //字母数字下划线是合法字符集
        }
    }
    if (instruct[3].size() > 30) throw error();
    for (char j : instruct[3]) {
        if (!Visible(j)) throw error();
    }
    account_file.RegisterAccount(instruct[1].c_str(), instruct[2].c_str(), instruct[3].c_str());
}

void passwd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status) {
    if (num > 4) throw error();
    if (num == 3) {
        for (int i = 1; i <= 2; ++i) {
            if (instruct[i].size() > 30) throw error();
            for (char j : instruct[i]) {
                if (!LetterNum_(j)) throw error();
            }
        }
        account_file.ChangePassword(instruct[1].c_str(), instruct[2].c_str(), log_status);
    } else {
        for (int i = 1; i <= 3; ++i) {
            if (instruct[i].size() > 30) throw error();
            for (char j : instruct[i]) {
                if (!LetterNum_(j)) throw error();
            }
        }
        account_file.ChangePassword(instruct[1].c_str(), instruct[3].c_str(), log_status, instruct[2].c_str());
    }
}

void useradd(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status) {
    if (num != 5) throw error();
    for (int i = 1; i <= 2; ++i) {
        if (instruct[i].size() > 30) throw error();
        for (const char &j: instruct[i]) {
            if (!LetterNum_(j)) throw error();
        }
    }
    if(instruct[3][0]=='0') throw error();
    int pri = stoi(instruct[3]);
    if (!(pri == 1 || pri == 3 || pri == 7)) throw error();
    if (instruct[4].size() > 30) throw error();
    for (const char &i: instruct[4]) {
        if (!Visible(i)) throw error();
    }
    if(log_status.login.empty() ||log_status.login.back().priority<3) throw error();
    account_file.CreatAccount(instruct[1].c_str(), instruct[2].c_str(), pri, instruct[4].c_str(), log_status);
}

void erase(const int &num, const std::vector<std::string> &instruct, AccountFile &account_file, LogStatus &log_status) {
    if (num != 2) throw error();
    if (instruct[1].size() > 30) throw error();
    for (char i : instruct[1]) {
        if (!LetterNum_(i)) throw error();
    }
    if(log_status.login.back().priority<7){  //需要权限等于7才能进行次操作
        std::cout<<"Invalid\n";
        return;
    }
    account_file.DeleteAccount(instruct[1].c_str(), log_status);
}

void show(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status, BookFile &book_file) {
    std::string info;
    std::string remain;
    if(log_status.login.empty() ||log_status.login.back().priority<1){  //要求目前登录栈栈尾的用户权限大于1
        std::cout<<"Invalid\n";
        return;
    }
    if (num == 1) {
        book_file.show_all();
    }else {
        bool flag = false;
        for (int i = 0; i < instruct[1].size(); ++i) {
            if (instruct[1][i] == '=') {
                flag = true;  //判断是否有等号出现
                info = std::string(instruct[1], 0, i + 1);
                remain = std::string(instruct[1], i + 1);
                break;
            }
        }
        if (!flag) throw error();
        if (info == "-ISBN=") {
            if (remain.size() > 20 || remain.empty()) throw error();  //判断ISBN合法性
            for (char &j: info) {
                if (!Visible(j)) throw error();
            }
            book_file.show_ISBN(remain.c_str());
        } else if (info == "-name=" || info == "-author=" || info == "-keyword=") {
            if (!(remain.front() == '"' && remain.back() == '"')) throw error();
            remain.erase(0, 1);
            remain.erase(remain.size() - 1, 1);  //去掉双引号
            if (remain.size() > 60 || remain.empty()) throw error();
            for (char &i: remain) {
                if (!Visible(i) || i == '"') throw error();
            }
            if (info == "-name=") {
                book_file.show_name(remain.c_str());
            } else if (info == "-author=") {
                book_file.show_author(remain.c_str());
            } else {
                for (char i: remain) if (i == '|') throw error();  //如果出现多个关键词，则操作失败
                book_file.show_keyword(remain.c_str());
            }
        } else {
            throw error();
        }
    }
}

void ShowFinance(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,TransactionLog &transaction_log) {
    if (log_status.login.back().priority != 7) throw error();
    if (num == 2) {
        transaction_log.ShowAll();
    } else if (num == 3) {
        for (char i : instruct[2]) {
            if (!('0' <= i && i <= '9')) throw error();
        }
        if(instruct[2][0]=='0' && instruct[2]!="0") throw error();
        int count = stoi(instruct[2]);
        if (count == 0) {
            std::cout << '\n';
        } else {
            transaction_log.Show(count);
        }
    }else{
        throw error();
    }
}

void modify(const int &num, const std::vector<std::string> &instruct, LogStatus &log_status,BookFile &book_file){
    if(log_status.login.empty() ||log_status.login.back().priority<3){  //要求目前登录栈栈尾的用户权限大于3
        std::cout<<"Invalid\n";
        return;
    }
    if(log_status.login.back().booknum==-1){   //如果未选中图书则操作失败
        std::cout<<"Invalid\n";
        return;
    }
    bool if_modify[5]={false,false,false,false,false};
    if(instruct.size()==1) throw error();
    for(int i=1;i< instruct.size();++i) {
        std::string info;  //ISBN/NAME/author/keyword/Price
        std::string remain;
        bool flag = false;
        for (int j = 0; j < instruct[i].size(); ++j) {
            if (instruct[i][j] == '=') {
                flag = true;  //判断是否有等号出现
                info = std::string(instruct[i], 0, j + 1);
                remain = std::string(instruct[i], j + 1);
                break;
            }
        }
        if (!flag) throw error();
        if (info == "-ISBN=") {
            if (if_modify[0]) throw error();  //有重复附加参数为非法指令；
            if (remain.size() > 20 || remain.empty()) throw error();  //判断ISBN合法性
            for (char &j: remain) {
                if (!Visible(j)) throw error();
            }
            if_modify[0] = true;
            book_file.modify_ISBN(remain.c_str(), log_status);
        } else if (info == "-name=" || info == "-author=" || info == "-keyword=") {
            if (!(remain.front() == '"' && remain.back() == '"')) throw error();
            remain.erase(0, 1);
            remain.erase(remain.size() - 1, 1);  //去掉双引号
            if (remain.size() > 60 || remain.empty()) throw error();
            for (const char k: remain) {
                if (!Visible(k) || k == '"') throw error();
            }
            if (info == "-name=") {
                if (if_modify[1]) throw error();
                book_file.modify_name(remain.c_str(), log_status);
                if_modify[1] = true;
            } else if (info == "-author=") {
                if (if_modify[2]) throw error();
                book_file.modify_author(remain.c_str(), log_status);
                if_modify[2] = true;
            } else {
                if (if_modify[3]) throw error();
                book_file.modify_keyword(remain.c_str(), log_status);
                if_modify[3] = true;
            }
        } else if (info == "-price=") {
            if (if_modify[4]) throw error();
            if (remain[0] == '0' && remain[1] != '.' && remain!="0") throw error();  //去除前导0，并且排除price=0的情况
            double price = stof(remain);
            book_file.modify_price(price, log_status);
            if_modify[4] = true;
        } else {
            throw error();
        }
    }
}


void import(const int &num, const std::vector<std::string> &instruct,  LogStatus &log_status,
            BookFile &book_file,TransactionLog& transaction_log){
    if(log_status.login.empty() ||log_status.login.back().priority<3){  //要求目前登录栈栈尾的用户权限大于3
        std::cout<<"Invalid\n";
        return;
    }
    if (num != 3) throw error();
    for (char i : instruct[1]) {
        if (!('0' <= i && i <= '9')) throw error(); //需要满足每一个数都在0-9之间；
    }
    if(instruct[1][0]=='0') throw error(); //去除前导0
    int quantity = stoi(instruct[1]);
    if(instruct[2][0]=='0' && instruct[2][1]!='.') throw error();
    for (char i : instruct[2]) {
        if (!(('0' <= i && i <= '9') || i == '.'))
            throw error(); //需要满足每一个数都在0-9之间,或者是.
    }
    double total_cost = stof(instruct[2]);
    if (total_cost == 0) throw error();
    book_file.import(quantity, total_cost, log_status, transaction_log);
}


void select (const int &num, const std::vector<std::string> &instruct,  LogStatus &log_status,
             BookFile &book_file){
    if(log_status.login.empty() || log_status.login.back().priority<3) throw error();
    if (num != 2) throw error();
    if (instruct[1].size() > 20) throw error();
    for (char i : instruct[1]) {
        if (!Visible(i)) throw error();
    }
    log_status.Select(instruct[1].c_str(), book_file);
}

void buy(const int &num, const std::vector<std::string> &instruct,  LogStatus &log_status,
         BookFile &book_file,TransactionLog &transaction_log){
    if(log_status.login.empty() ||log_status.login.back().priority<1){  //要求目前登录栈栈尾的用户权限大于1
        throw error();
    }
    if (num != 3) throw error();
    if ((instruct[1]).size() > 20) throw error();
    for (char i : instruct[1]) {
        if (!Visible(i)) throw error();  //保证均为可见ascii字符
    }
    for (char i : instruct[2]) {
        if (!('0' <= i && i <= '9')) throw error(); //需要满足每一个数都在0-9之间；
    }
    if(instruct[2][0]=='0') throw error(); //去除前导0或者0
    int quantity = stoi(instruct[2]);
    book_file.buy(instruct[1].c_str(), quantity,transaction_log);
}