#ifndef BOOKSTORE_UNROLLLINKLIST_H
#define BOOKSTORE_UNROLLLINKLIST_H

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>
#include <set>

const int maxsize=300;
const int minsize=150;
class unroll_link;
class block_node;


char *min(char *a,char *b);

char *max(char *a,char *b);

long to_block_pos(int pos);

long to_array_pos(int pos);

int array_pos_to_pos(long array_pos);

int block_pos_to_pos(long block_pos);

struct data{
    char index[70];
    int value;
    data()=default;
    data(const char *,int);
};
struct block_node {

    long next;//下一个节点信息在node_file里的地址
    long last;//上一个节点信息在node_file里的地址,注意不能直接存指针，因为一个内存地址可能随时改变，存在外存文件里是没有意义的。
    int pos;//表示对应的数组存在外存的位置标号以及结点本身的位置标号（一一对应） //0-base
    int size;//目前数组中元素的个数
    char minimum [70];
    char maximum [70];
//    bool if_empty;
    block_node()=default;

    block_node(long _next, long _last,int  pos);

    ~block_node();

    void BlockMerge(block_node *y,const unroll_link& _unrollLink); //将x和y合并成x,把y丢弃

    void BlockSplit(unroll_link& _unrollLink);

    std::vector<int> TryFindInArray(const char *index,unroll_link &unrollLink) const;

    void InsertInArray(const data&_data,const unroll_link& _unrollLink);

    void EraseInArray(int position,data * temp_data,unroll_link& _unrollLink);

};
class unroll_link{
private:

    friend struct block_node;

    const std::string filename;
    const std::string node_filename;
    const std::string recycle_filename;
    block_node head;
    block_node tail;
    int block_size;
    int recycle_size;

    int TryFind(const char *index);

public:
    unroll_link(std::string name,std::string node_name,const std::string& recyle_name);

    ~unroll_link();


    void InsertInBlock(const char *index,const int &value);

    void EraseInBlock(const char *index,const int &value);

    std::string FindInBlock(const char *index);

};




#endif
