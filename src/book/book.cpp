#include "book.h"

bool operator<(const BookInfo &x, const BookInfo &y) {
    return strcmp(x.ISBN, y.ISBN) < 0;
}

BookFile::BookFile() : isbn_num("file/isbn", "file/isbn_o"), bookname_num("file/bookname", "file/bookname_o"),
                       author_num("file/author", "file/author_o"), keyword_num("file/keyword", "file/keyword_o") {
    std::ifstream in("file/book_file");
    if (!in) {
        std::ofstream out("file/book_file");  //  如果还是第一次的话，创建这个文件
        out.close();
        iof.open("file/book_file", std::fstream::in | std::fstream::out);
//        book_total_num = 0;
    } else {
        iof.open("file/book_file", std::fstream::in | std::fstream::out);
//        iof.read(reinterpret_cast<char *>(&book_total_num), sizeof(int));
    }
    in.close();
}

BookFile::~BookFile() {
    iof.seekp(0);
//    iof.write(reinterpret_cast<char *>(&book_total_num), sizeof(int));
    iof.close();
}


void BookFile::show_ISBN(const char *ISBN) {
    std::vector<int> num_vec = isbn_num.FindInBlock(ISBN);
    if (!num_vec.empty()) {
        std::set<BookInfo> temp_set;
        for (const int &it: num_vec) {
            BookInfo temp{};
            iof.seekg(it);
            iof.read(reinterpret_cast<char *>(&temp), sizeof(BookInfo));
            temp_set.insert(temp);
        }
        for (const BookInfo &it: temp_set) {
            std::cout << it.ISBN << '\t' << it.name << '\t' << it.author << '\t' << it.org_keywords << '\t';
            double real_price = (double) it.price / 100.00;
            printf("%.2f", real_price);
            std::cout << '\t' << it.quantity << '\n';
        }

    } else {
        std::cout << '\n';
    }
}

void BookFile::show_name(const char *name) {
    std::vector<int> num_vec = bookname_num.FindInBlock(name);
    if (!num_vec.empty()) {
        std::set<BookInfo> temp_set;
        for (const int &it: num_vec) {
            BookInfo temp{};
            iof.seekg(it);
            iof.read(reinterpret_cast<char *>(&temp), sizeof(BookInfo));
            temp_set.insert(temp);
        }
        for (const BookInfo &it: temp_set) {
            std::cout << it.ISBN << '\t' << it.name << '\t' << it.author << '\t' << it.org_keywords << '\t';
            double real_price = (double) it.price / 100.00;
            printf("%.2f", real_price);
            std::cout << '\t' << it.quantity << '\n';
        }

    } else {
        std::cout << '\n';
    }
}

void BookFile::show_author(const char *author) {
    std::vector<int> num_vec = author_num.FindInBlock(author);
    if (!num_vec.empty()) {
        std::set<BookInfo> temp_set;
        for (const int &it: num_vec) {
            BookInfo temp{};
            iof.seekg(it);
            iof.read(reinterpret_cast<char *>(&temp), sizeof(BookInfo));
            temp_set.insert(temp);
        }
        for (const BookInfo &it: temp_set) {
            std::cout << it.ISBN << '\t' << it.name << '\t' << it.author << '\t' << it.org_keywords << '\t';
            double real_price = (double) it.price / 100.00;
            printf("%.2f", real_price);
            std::cout << '\t' << it.quantity << '\n';
        }

    } else {
        std::cout << '\n';
    }
}

void BookFile::show_keyword(const char *keyword) {
    std::vector<int> num_vec = keyword_num.FindInBlock(keyword);
    if (!num_vec.empty()) {
        std::set<BookInfo> temp_set;
        for (const int &it: num_vec) {
            BookInfo temp{};
            iof.seekg(it);
            iof.read(reinterpret_cast<char *>(&temp), sizeof(BookInfo));
            temp_set.insert(temp);
        }
        for (const BookInfo &it: temp_set) {
            std::cout << it.ISBN << '\t' << it.name << '\t' << it.author << '\t' << it.org_keywords << '\t';
            double real_price = (double) it.price / 100.00;
            printf("%.2f", real_price);
            std::cout << '\t' << it.quantity << '\n';
        }
    } else {
        std::cout << '\n';
    }
}

void BookFile::show_all() {
    std::vector<int> pos = isbn_num.FindInBlock("");
    for (int i: pos) {
        BookInfo temp_book{};
        iof.seekg(i);
        iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo));
        std::cout << temp_book.ISBN << '\t' << temp_book.name << '\t' << temp_book.author << '\t'
                  << temp_book.org_keywords << '\t';
        double real_price = (double) temp_book.price / 100.00;
        printf("%.2f", real_price);
        std::cout << '\t' << temp_book.quantity << '\n';
    }
}

void BookFile::buy(const char *ISBN, const int &quantity, TransactionLog &transaction_log, LogStatus &log_status,
                   AccountFile &accountFile) {
    std::vector<int> num_vec = isbn_num.FindInBlock(ISBN);
    if (num_vec.empty()) {
        std::cout << "Invalid\n";
        return;
    }
    iof.seekg(num_vec[0]);
    BookInfo temp{};
    iof.read(reinterpret_cast<char *>(&temp), sizeof(BookInfo));
    if (temp.quantity >= quantity) {
        temp.quantity -= quantity;
        iof.seekp(num_vec[0]);
        iof.write(reinterpret_cast<char *>(&temp), sizeof(BookInfo));
        long long earn = (long long) quantity * temp.price;
        double real_earn = (double) earn / 100.00;
        printf("%.2f\n", real_earn);
        transaction_log.earn(earn);
        AccountInfo _operator{};
        accountFile.iof.seekg(log_status.login.back().usernum);
        accountFile.iof.read(reinterpret_cast<char *>(&_operator), sizeof(AccountInfo));
        std::string info = "Book sold: ISBN= ";
        info.append(ISBN);
        info += " bookname= ";
        info.append(temp.name);
        info += " quantity= ";
        info += IntToString(quantity);
        transaction_log.AddRecord(info.c_str(), _operator.UserId, _operator.priority);
    } else {
        std::cout << "Invalid\n";
    }
}

void BookFile::ModifyISBN(const char *ISBN, LogStatus &log_status) {
    BookInfo temp_book{};
    iof.seekg(log_status.login.back().booknum);
    iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //读入当前被选中的图书的信息
    isbn_num.EraseInBlock(temp_book.ISBN, log_status.login.back().booknum);//修改块状链表中的内容
    isbn_num.InsertInBlock(ISBN, log_status.login.back().booknum);
    strcpy(temp_book.ISBN, ISBN);
    iof.seekp(log_status.login.back().booknum);
    iof.write(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo));  //修改book_file的内容
}

void BookFile::Modifyname(const char *name, LogStatus &log_status) {
    BookInfo temp_book{};
    iof.seekg(log_status.login.back().booknum);
    iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //读入当前被选中的图书的信息
    bookname_num.EraseInBlock(temp_book.name, log_status.login.back().booknum); //修改块状链表中的信息
    bookname_num.InsertInBlock(name, log_status.login.back().booknum);
    strcpy(temp_book.name, name);
    iof.seekp(log_status.login.back().booknum);  //读指针和写指针是完全独立的！
    iof.write(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo));  //修改book_file中的内容
}

void BookFile::Modifyauthor(const char *author, LogStatus &log_status) {
    BookInfo temp_book{};
    iof.seekg(log_status.login.back().booknum);
    iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //读入当前被选中的图书的信息
    author_num.EraseInBlock(temp_book.author, log_status.login.back().booknum); //修改块状链表中的信息
    author_num.InsertInBlock(author, log_status.login.back().booknum);
    strcpy(temp_book.author, author);
    iof.seekp(log_status.login.back().booknum);
    iof.write(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo));
}

void BookFile::Modifykeyword(const char *keyword, LogStatus &log_status) {
    std::vector<std::string> keyword_vec = ProcessKeywords(keyword);
    BookInfo temp_book{};
    iof.seekg(log_status.login.back().booknum);
    iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //读入当前被选中的图书的信息
    std::vector<std::string> temp_key_vec = ProcessKeywords(temp_book.org_keywords);
    for (std::string &it: temp_key_vec) {  //开始操作：删去所有本来的关键词
        keyword_num.EraseInBlock(it.c_str(), log_status.login.back().booknum);
    }
    for (std::string &it: keyword_vec) {  //加入新的关键词
        keyword_num.InsertInBlock(it.c_str(), log_status.login.back().booknum);
    }
    strcpy(temp_book.org_keywords, keyword);
    iof.seekp(log_status.login.back().booknum);  //更新书籍信息
    iof.write(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo));
}

void BookFile::Modifyprice(long long price, LogStatus &log_status) {
    BookInfo temp_book{};
    iof.seekg(log_status.login.back().booknum);
    iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //读入当前被选中的图书的信息
    temp_book.price = price;
    iof.seekp(log_status.login.back().booknum);
    iof.write(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //写入更新后的图书信息
}

void BookFile::import(int quantity, long long cost, LogStatus &log_status, TransactionLog &transaction_log,
                      LogStatus &logStatus, AccountFile &accountFile) {
    if (log_status.login.back().booknum == -1) {   //如果未选中图书则操作失败
        std::cout << "Invalid\n";
        return;
    }
    BookInfo temp_book{};
    iof.seekg(log_status.login.back().booknum);
    iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //读入当前被选中的图书的信息
    temp_book.quantity += quantity;
    iof.seekp(log_status.login.back().booknum);
    iof.write(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //写入更新后的图书信息
    transaction_log.cost(cost);
    AccountInfo _operator{};
    accountFile.iof.seekg(log_status.login.back().usernum);
    accountFile.iof.read(reinterpret_cast<char *>(&_operator), sizeof(AccountInfo));
    std::string info = "Book imported: ISBN= ";
    info.append(temp_book.ISBN);
    info += " bookname= ";
    info.append(temp_book.name);
    info += " quantity= ";
    info += IntToString(quantity);
    transaction_log.AddRecord(info.c_str(), _operator.UserId, _operator.priority);
}

void BookFile::TryModifyISBN(const char *ISBN, LogStatus &log_status) {
    BookInfo temp_book{};
    iof.seekg(log_status.login.back().booknum);
    iof.read(reinterpret_cast<char *>(&temp_book), sizeof(BookInfo)); //读入当前被选中的图书的信息
    if (strcmp(temp_book.ISBN, ISBN) == 0) {   //不允许将ISBN修改为原有的ISBN
        throw error();
    }
    if (!isbn_num.FindInBlock(ISBN).empty()) {  //任何两本书的ISBN不能重复
        throw error();
    }
}

void BookFile::TryModifyname(const char *name, LogStatus &log_status) {
}

void BookFile::TryModifyauthor(const char *author, LogStatus &log_status) {

}

void BookFile::TryModifykeyword(const char *keyword, LogStatus &log_status) {
    std::vector<std::string> keyword_vec = ProcessKeywords(keyword);
    if (keyword_vec.empty()) {  //确保更新的关键词信息是合法的
        throw error();
    }
}

void BookFile::TryModifyprice(long long price, LogStatus &log_status) {

}






//
//std::vector<int> BookFile::isbn_to_num(const char *isbn) {
//    std::vector<int> ans;
//    ans=isbn_num.FindInBlock(isbn);
//    return ans;
//}



