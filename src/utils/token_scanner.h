
#ifndef _BOOKSTORE_TOKENSCANNER_H
#define _BOOKSTORE_TOKENSCANNER_H

#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <iostream>
#include "../bookstore.h"
long long DoubleStringToll(const std::string &s);    //为了防止丢失精度，把所有浮点数都存成整数。

std::vector<std::string>ProcessKeywords(const char *);

int StringToInt(const std::string &s);

std::string IntToString(int a);

bool LetterNum_(const char &x); //判断是否是只有字母或数字或下划线  UserId,Password

bool Visible(const char &x); //判断是都均为可见ASCII字符  Username



#endif //_BOOKSTORE_TOKENSCANNER_H
