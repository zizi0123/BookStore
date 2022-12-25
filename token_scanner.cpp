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


/*********************************************************************************************************/
//tokenscanner 需要顺便检查数据类型的正确性