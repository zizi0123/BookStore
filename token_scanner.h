//

//

#ifndef _BOOKSTORE_TOKENSCANNER_H
#define _BOOKSTORE_TOKENSCANNER_H

#include <string>
#include <vector>
#include <cstring>
#include <cctype>

class TokenScanner{
public:
    TokenScanner()=default;
//    TokenScanner(std::string str);
    ~TokenScanner()=default;
//
//    void setInput(std::string str);
//
//    bool HasMoreTokens();
//
//    std::string NextToken();
private:
    std::string original_str;
};

std::vector<std::string>ProcessKeywords(const char *);
#endif //_BOOKSTORE_TOKENSCANNER_H
