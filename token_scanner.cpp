#include "token_scanner.h"
std::vector<std::string>ProcessKeywords(const char *keyword){
    int n=0;
    int start=0;
    std::vector<std::string> ans;
    for(int i=0;i<strlen(keyword);++i){
        std::string key;
        if(keyword[i]=='|' || i==strlen(keyword)-1){
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

int DoubleStringToInt(const std::string &s) {   //将一个浮点数字符串保存成一个保留到两位小数的整数。
    int a=0, b=0; //小数部分和整数部分
    int num_dot = -1;  //小数点的位数
    for (int i = 0; i < s.size() - 1; ++i) {
        if (s[i] == '.') {
            num_dot = i;
            break;
        }
    }
    int ans = 0;
    if(num_dot!=-1) {  //说明有小数部分
        int digit=1;
        for (int i = num_dot - 1; i >= 0; --i) {  //将整数部分转成整数
            a += (s[i]-'0') * digit;
            digit *= 10;
        }
        if(num_dot+1==s.size()-1){  //如果只有一位小数
            return 100*a+(s.back()-'0')*10;
        }else{
            return 100*a+(s[num_dot+1]-'0')*10+(s[num_dot+2]-'0');
        }
    }else {  //说明输入的价格就是一个整数
        return 100*stoi(s);
    }

}

//double StringtoDouble(const std::string &s) {   //浮点数是用无穷二进制小数存储的，如果对浮点数进行很多操作，可能会丢失精度。
//    double x;
//    int a=0, b=0; //小数部分和整数部分
//    int num_dot = -1;  //小数点的位数
//    for (int i = 0; i < s.size() - 1; ++i) {
//        if (s[i] == '.') {
//            num_dot = i;
//            break;
//        }
//    }
//    double ans = 0;
//    if(num_dot!=-1) {  //说明有小数部分
//        int digit=1;
//        for (int i = num_dot - 1; i >= 0; --i) {  //将整数部分转成整数
//            a += (s[i]-'0') * digit;
//            digit *= 10;
//        }
//        digit=1;
//        for(int i=s.size()-1;i>num_dot;--i){  //将小数部分转成整数
//            b+=(s[i]-'0')*digit;
//            digit*=10;
//        }
//        ans=a+1.000/(pow(10,(s.size()-1-num_dot)))*b;
//    }else {  //说明输入的价格就是一个整数
//        ans = (double) stoi(s);
//    }
//    return ans;
//}
/*********************************************************************************************************/
//tokenscanner 需要顺便检查数据类型的正确性