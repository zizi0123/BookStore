#include "unrolllinklist.h"
#include <iostream>


int main() {
//    freopen("in","r",stdin);
//    freopen("out","w",stdout);
    int num;
    std::cin >> num;
    unroll_link unroll_linkk("text", "other_information");
    char x;
    std::cin.get(x);
    for (int i = 1; i <= num; ++i) {
        char *command = new char[100];
        std::cin.getline(command, 100);
        char info[10];
        int j1 = 0;
        for (; j1 < strlen(command); ++j1) {
            if (command[j1] == ' ') break;
        }
        strncpy(info, command, j1);
        info[j1] = '\0';
        if (strcmp("insert", info) == 0 || strcmp("delete", info) == 0) {
            char index[70], value[15];
            int j2 = 0;
            for (; j2 < strlen(command); ++j2) {
                if (command[j2 + j1 + 1] == ' ') break;
            }
            strncpy(index, command + j1 + 1, j2);
            index[j2] = '\0';
            strcpy(value, command + j1 + j2 + 2);
            value[strlen(command) - (j1 + j2 + 2)] = '\0';
            int int_value = atoi(value);
            if (strcmp("insert", info) == 0) {
                unroll_linkk.InsertInBlock(index, int_value);
            } else {
                unroll_linkk.EraseInBlock(index, int_value);
            }
        } else if(strcmp("find", info) == 0){
            char index[70];
            int k=j1+1;
            while(true){
                if(command[k]==' ' || command[k]=='\0' ||command[k]=='\n'){
                    strncpy(index,command+j1+1,k-j1-1);
                    index[k-j1-1] = '\0';
                    break;
                }
                k++;
            }
            std::string ans = unroll_linkk.FindInBlock(index);
            std::cout << ans;
        }
        delete[] command;
    }
}

