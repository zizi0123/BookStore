#include "account.h"

AccountInfo::AccountInfo(const char *Id, const char *_name, const char *_password, int _priority):priority(_priority) {
    strcpy(UserId,Id);
    strcpy(name,_name);
    strcpy(password,_password);
}


AccountFile::AccountFile(): id_num("file/id","file/id_o") {
    std::ifstream in("file/account_file");
    if(!in){  //如果是第一次，就创建这个文档，并且加入超级管理员信息
        std::ofstream out("file/account_file"); //如果还是第一次操作的话，创建一个文件
        out.close();
        iof.open("file/account_file", std::fstream::in | std::fstream::out);
        AccountInfo super_user("root", "root", "sjtu", 7);
        int super_num = 0;
        iof.seekp(super_num);
        iof.write(reinterpret_cast<char *>(&super_user), sizeof(AccountInfo)); //写入超级管理员的信息
        id_num.InsertInBlock(super_user.UserId,0);
    }else{
        iof.open("file/account_file", std::fstream::in | std::fstream::out);
    }
    in.close();
}

AccountFile::~AccountFile() {
    iof.close();
}

void AccountFile::RegisterAccount(const char *UserId, const char *password, const char *name,TransactionLog& transactionLog) {  //添加一个新账户
    std::vector<int> find_ans=id_num.FindInBlock(UserId);
    if(!find_ans.empty()){  //ID不能与已注册账户重复
        std::cout<<"Invalid\n";
        return;
    }
    AccountInfo new_account(UserId,name,password,1);  //创建新账户
    iof.seekp(0,std::fstream::end);
    int new_pos=(int)iof.tellp();
    iof.write(reinterpret_cast<char *>(&new_account),sizeof(AccountInfo));
    id_num.InsertInBlock(UserId,new_pos);
    std::string info="Register: Id= ";
    info.append(UserId);
    info+=" password= ";
    info.append(password);
    info+=" name= ";
    info.append(name);
    transactionLog.AddRecord(info.c_str(),"",-1);
}

void AccountFile::ChangePassword(const char *UserId, const char *new_password, LogStatus &log_status,TransactionLog& transactionLog,const char *current_password) {
    if(log_status.login.empty() ||log_status.login.back().priority<1){  //要求：权限大于1
        std::cout<<"Invalid\n";
        return;
    }
    std::vector<int> user_num_vec=id_num.FindInBlock(UserId);
    if(user_num_vec.empty()){  //账户不存在：非法操作
        std::cout<<"Invalid\n";
        return;
    }
    int user_num=user_num_vec[0];
    AccountInfo temp_account{};
    iof.seekg(user_num);
    iof.read(reinterpret_cast<char *>(&temp_account),sizeof (AccountInfo));
    if(strcmp(current_password,"")==0){  //如果没有输入当前密码，就要求权限必须为7
        if(log_status.login.back().priority<7){
            std::cout<<"Invalid\n";
            return;
        }
    }else if(strcmp(current_password,temp_account.password)!=0){  //密码错误
        std::cout<<"Invalid\n";
        return;
    }
    strcpy(temp_account.password,new_password);  //更改信息
    iof.seekp(user_num);
    iof.write(reinterpret_cast<char *>(&temp_account),sizeof (AccountInfo));
    AccountInfo _operator{};
    iof.seekg(log_status.login.back().usernum);
    iof.read(reinterpret_cast<char *>(&_operator),sizeof(AccountInfo));
    std::string info="ChangePassword: Id= ";
    info.append(temp_account.UserId);
    info+=" name= ";
    info.append(temp_account.name);
    info+=" to new password= ";
    info.append(new_password);
    transactionLog.AddRecord(info.c_str(),_operator.UserId,_operator.priority);
}

void AccountFile::CreatAccount(const char *UserId, const char *password,const int priority, const char *name,  LogStatus &log_status,TransactionLog& transactionLog) {
    if(priority>=log_status.login.back().priority){  //带创建的用户的权限大于等于当前权限，非法！
        std::cout<<"Invalid\n";
        return;
    }
    std::vector<int> user_num_vec=id_num.FindInBlock(UserId);
    if(!user_num_vec.empty()){  //UserId与已注册账户重复！
        std::cout<<"Invalid\n";
        return;
    }
    AccountInfo new_account (UserId,name, password,priority);
    iof.seekp(0,std::fstream::end);
    int user_num=(int)iof.tellp();
    iof.write(reinterpret_cast<char *>(&new_account),sizeof (AccountInfo));  //写入新账户的信息
    id_num.InsertInBlock(UserId,user_num);
    AccountInfo _operator{};
    iof.seekg(log_status.login.back().usernum);
    iof.read(reinterpret_cast<char *>(&_operator),sizeof(AccountInfo));
    std::string info="CreatAccount: Id= ";
    info.append(UserId);
    info+=" name= ";
    info.append(name);
    info+=" password= ";
    info.append(password);
    transactionLog.AddRecord(info.c_str(),_operator.UserId,_operator.priority);
}

void AccountFile::DeleteAccount(const char *UserId,LogStatus &log_status,TransactionLog& transaction_log) {
    std::vector<int> user_num_vec=id_num.FindInBlock(UserId);
    if(user_num_vec.empty()){  //待删除用户不存在，操作非法！
        std::cout<<"Invalid\n";
        return;
    }
    int user_num=user_num_vec[0];
    for(const LogInfo &it:log_status.login){ //如果待删除账户已登录，操作非法
        if(user_num==it.usernum){
            std::cout<<"Invalid\n";
            return;
        }
    }
    AccountInfo deleted{};
    iof.seekg(user_num);
    iof.read(reinterpret_cast<char *>(&deleted),sizeof(AccountInfo));
    id_num.EraseInBlock(UserId,user_num);
    AccountInfo _operator{};
    iof.seekg(log_status.login.back().usernum);
    iof.read(reinterpret_cast<char *>(&_operator),sizeof(AccountInfo));
    std::string info="DeleteAccount: Id= ";
    info.append(UserId);
    info+=" name= ";
    info.append(deleted.name);
    info+=" password= ";
    info.append(deleted.password);
    transaction_log.AddRecord(info.c_str(),_operator.UserId,_operator.priority);
}





