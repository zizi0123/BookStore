#include "token_scanner.h"


int StringToInt(const std::string &s){
    if(s.front()=='0' && s!="0"){  //排除前导0
        throw error();
    }
    for(const char &i:s){
        if(!('0'<=i && i<='9')){
            throw error();
        }
    }
    int ans;
    try {
        ans= stoi(s);
    }catch (...){
        throw error();
    }
    return ans;
}

std::vector<std::string>ProcessKeywords(const char *keyword){
    int n=0;
    int start=0;
    std::vector<std::string> ans;
    for(int i=0;i<strlen(keyword);++i){
        std::string key;
        if(keyword[i]=='|' || i==strlen(keyword)-1){  //到达一串信息的末端
            if(i==strlen(keyword)-1) n++;  //补上一位
            if(n==0){  //，每一段key的程度至少是1
                ans.clear();
                return ans;
            }
            key=std::string (keyword,start,n);
            start=i+1;
            n=0;
            for(const std::string &it:ans){
                if(it==key){  //不能出现重复的key
                    ans.clear();
                    return ans;
                }
            }
            ans.push_back(key);
        }else{
            n++;
        }
    }
    return ans;
}

long long DoubleStringToll(const std::string &s) {   //将一个浮点数字符串保存成一个保留到两位小数的整数。
    if (s.front() == '0' && s[1] != '.'){ //去除前导0
       if(s!="0") throw error(); //price=0合法
    }
    long long a=0; //整数部分
    int num_dot = -1;  //小数点的位数
    for (int i = 0; i < s.size() - 1; ++i) {
        if(!(('0'<=s[i] && s[i]<='9')|| s[i]=='.')) throw error();
        if (s[i] == '.') {
            if(num_dot==-1) {
                num_dot = i;
            }else{
                throw error();  //说明出现多次点，非法
            }
        }
    }
    if(num_dot!=-1) {  //说明有小数部分
        int digit=1;
        for (int i = num_dot - 1; i >= 0; --i) {  //将整数部分转成整数
            a += (long long)(s[i]-'0') * digit;
            digit *= 10;
        }
        if(num_dot+1==s.size()-1){  //如果只有一位小数
            return 100*a+(long long)(s.back()-'0')*10;
        }else{
            return 100*a+(long long)(s[num_dot+1]-'0')*10+(long long)(s[num_dot+2]-'0');
        }
    }else {  //说明输入的价格就是一个整数
        return 100*(long long)stoi(s);
    }

}


/*********************************************************************************************************/
