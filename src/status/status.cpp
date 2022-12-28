#include "status.h"

void LogStatus::SwitchUser(const char *UserId,AccountFile& account_file,TransactionLog &transaction_log,const char *password) {
    LogInfo temp{};
    AccountInfo temp_account{};
    std::vector<int> account_num=account_file.id_num.FindInBlock(UserId);
    if(account_num.empty()){  //如果该账户不存在，则操作失败
        std::cout<<"Invalid\n";
        return;
    }
    account_file.iof.seekg(account_num[0]);
    account_file.iof.read(reinterpret_cast<char *>(&temp_account),sizeof(AccountInfo));

    if(strcmp(password,"")==0){  //如果没有输入密码， 要求当前账户权限高于登录账户
        if(login.empty() || login.back().priority<=temp_account.priority){
            std::cout<<"Invalid\n";
            return;
        }
    }else {
        if(strcmp(temp_account.password,password)!=0){
            std::cout<<"Invalid\n";
            return;
        }
    }
    temp.priority=temp_account.priority;
    temp.booknum=-1;
    temp.usernum=account_num[0];
    login.push_back(temp);
    std::string  info="User Login: Id= ";
    info.append(temp_account.UserId);
    info+=" name= ";
    info.append(temp_account.name);
    info+=" privilege= ";
    info.push_back('0'+temp_account.priority);
    transaction_log.AddRecord(info.c_str());
}

void LogStatus::logout(TransactionLog &transaction_log,AccountFile&accountFile) {
    if(login.empty()){
        std::cout<<"Invalid\n";
    }else{
        AccountInfo temp{};
        accountFile.iof.seekg(login.back().usernum);
        accountFile.iof.read(reinterpret_cast<char *>(&temp),sizeof (AccountInfo));
        std::string  info="User Logout: Id= ";
        info.append(temp.UserId);
        info+=" name= ";
        info.append(temp.name);
        info+=" privilege= ";
        info.push_back('0'+temp.priority);
        transaction_log.AddRecord(info.c_str());
        login.pop_back();
    }
}

void LogStatus::Select(const char *ISBN, BookFile &book_file) {
    std::vector<int> book_num=book_file.isbn_num.FindInBlock(ISBN);
    if(book_num.empty()){ //如果这本书不存在，就创造一本新书
        BookInfo temp_book{};
        strcpy(temp_book.ISBN,ISBN);
        strcpy(temp_book.author,"");
        strcpy(temp_book.org_keywords,"");
        strcpy(temp_book.name,"");
        temp_book.price=0;
        temp_book.quantity=0;
        book_file.iof.seekp(0,std::fstream ::end);
        int new_book_num=book_file.iof.tellp();
        book_file.iof.write(reinterpret_cast<char *>(&temp_book),sizeof (BookInfo));  //将新书的信息写入
        book_file.isbn_num.InsertInBlock(ISBN,new_book_num);
        login.back().booknum=new_book_num;
    }else{
        login.back().booknum=book_num[0];
    }
}


