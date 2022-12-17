//

//

#ifndef _BOOKSTORE_TOKENSCANNER_H
#define _BOOKSTORE_TOKENSCANNER_H

#include <string>

class TokenScanner{
public:
    TokenScanner();
    TokenScanner(std::string str);
    ~TokenScanner();

    void setInput(std::string str);

    bool HasMoreTokens();

    std::string NextToken();
private:
    std::string original_str;
};
#endif //_BOOKSTORE_TOKENSCANNER_H
