//
// Created by 86180 on 2022/12/9.
//

#ifndef _BOOKSTORE_BOOK_H
#define _BOOKSTORE_BOOK_H
#include <iostream>
#include <fstream>
struct book_info{
    char ISBN[21];
    char name[61];
    char author[61];
    char keywords[61];
    int quantity;
    double price;
};
class book{
public:
    std::string show_ISBN(std::string ISBN);
    std::string show_name(std::string name);
    std::string show_author(std::string author);
    std::string show_keyword(std::string keyword);
    void buy(std::string ISBN,std::string quantity);
    void modify_ISBN(std::string ISBN);
    void modify_name(std::string name);
    void modify_author(std::string author);
    void modify_keyword(std::string keyword);
    void modify_price(double prize);
    void import(int quantity,double cost);

};
class book_file{

};
#endif //_BOOKSTORE_BOOK_H
