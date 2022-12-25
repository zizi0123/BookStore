#ifndef BOOKSTORE_UNROLLLINKLIST_H
#define BOOKSTORE_UNROLLLINKLIST_H

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>
#include <set>
const int maxsize = 600;
const int minsize = 300;

class UnrollLink;

struct data {
    char index[70];
    int value;

    data() = default;

    data(const char *, int);

    data(const data &x);

    data &operator=(const data &x);
};

bool operator<(const data &x, const data &y);

bool operator==(const data &x,const data &y);

bool operator<=(const data &x,const data &y);

struct block_node {

    long pos;//表示对应的数组存在外存的位置 //0-base
    int size;//目前数组中元素的个数
    data minimum;
    data maximum;

    block_node(long _pos, int size);

    block_node() = default;

};

class UnrollLink {
private:

    friend struct block_node;

    const std::string filename;
    const std::string other_information_file;
    int block_size;
    int recycle_size;
    std::vector<block_node> head_list;
    std::fstream iof;

    int FindBlockNumIns(const data &x);

    std::vector<int> FindBlockNumFind(const char *_index);

    long FindBlockNumEra(const data &index);

    void InsertInArray(const data &_data, const int &);

    void BlockSplit(const long &position);

    void EraseInArray(long block_num, int position, data *temp_data);

    void BlockMerge(const long &block_num); //将x和y合并成x,把y丢弃

    std::vector<int> FindInArray(const std::vector<int> &block_num, const char *index);


public:
    UnrollLink(std::string name, std::string other_name);

    ~UnrollLink();

    void InsertInBlock(const char *index, const int &value);

    void EraseInBlock(const char *index, const int &value);

    std::vector<int> FindInBlock(const char *index);

};


#endif
