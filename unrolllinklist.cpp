#include "unrolllinklist.h"
#include <algorithm>
#include <utility>

std::vector<long> recycle_bin; //储存现在是空的的位置


data::data(const char *_index, int _value) : value(_value) {
    strcpy(index, _index);
}

data &data::operator=(const data &x) {
    if (this == &x) return *this;
    value = x.value;
    strcpy(index, x.index);
    return *this;
}

bool operator<=(const data &x, const data &y) {
    return (x < y || x == y);
}


data::data(const data &x) {
    value = x.value;
    strcpy(index, x.index);
}

bool operator<(const data &x, const data &y) {
    if (strcmp(x.index, y.index) != 0) return strcmp(x.index, y.index) < 0;
    return x.value < y.value;
}

bool operator==(const data &x, const data &y) {
    return (strcmp(x.index, y.index) == 0 && x.value == y.value);
}

block_node::block_node(long _pos, int _size) : pos(_pos), size(_size) {}


void unroll_link::BlockMerge(const long &block_num) {  //将第block_num与第block_num+1个合成为block_num

    data *temp = new data[head_list[block_num + 1].size]; //进行处理：数据迁移
    iof.seekg(head_list[block_num + 1].pos);
    for (int i = 0; i < head_list[block_num + 1].size; ++i) {  //把y中的数据读到数组temp里
        iof.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    iof.seekp(head_list[block_num].pos + head_list[block_num].size * sizeof(data));
    for (int i = 0; i < head_list[block_num + 1].size; ++i) {
        iof.write(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    delete[]temp;
    head_list[block_num].size += head_list[block_num + 1].size;
    head_list[block_num].maximum = head_list[block_num + 1].maximum;
    recycle_bin.push_back(head_list[block_num + 1].pos);
    head_list.erase(head_list.begin() + block_num + 1);
    --block_size;
}

void
unroll_link::BlockSplit(const long &position) {  //在第position个块的后面新插入一个块,给第position个留下minsize个元素，从size-minsize开始的元素赋给y
    long y_pos; //确定y的数组的起始位置
    if (!recycle_bin.empty()) {
        y_pos = recycle_bin.back();
        recycle_bin.pop_back();
    } else {
        iof.seekg(sizeof(data) * block_size * maxsize);
        block_size++;
        y_pos = iof.tellg();
    }
    block_node y(y_pos, maxsize - minsize); //需要新插入的块
    data this_max;
    iof.seekg(head_list[position].pos + (minsize - 1) * sizeof(data));
    iof.read(reinterpret_cast<char *>(&this_max), sizeof(data));
    data *temp = new data[maxsize - minsize]; //读入需要迁移的信息
    for (int i = 0; i < maxsize - minsize; ++i) {
        iof.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    iof.seekp(y.pos);
    for (int i = 0; i < maxsize - minsize; ++i) {
        iof.write(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    y.maximum = temp[maxsize - minsize - 1];
    y.minimum = temp[0];
    head_list.insert(head_list.begin() + position + 1, y); //完成对新块的操作
    delete[]temp;
    head_list[position].maximum = this_max;
    head_list[position].size = minsize;
}

void unroll_link::InsertInArray(const data &_data, const int &block_num) { //无需考虑是否会裂块!
    data *temp = new data[head_list[block_num].size];
    iof.seekg(head_list[block_num].pos);
    for (int i = 0; i < head_list[block_num].size; ++i) {
        iof.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    if (head_list[block_num].maximum < _data) {  //特殊情况：比最后maximum还大，插在最后
        iof.seekp(head_list[block_num].pos + head_list[block_num].size * sizeof(data));
        iof.write(reinterpret_cast<const char *>(&_data), sizeof(data));
        head_list[block_num].maximum = _data;
    } else {
        long proper_pos = std::lower_bound(temp, temp + head_list[block_num].size - 1, _data) - temp;  //pos是这个数应该被插到的位置
        iof.seekp(head_list[block_num].pos + proper_pos * sizeof(data));
        iof.write(reinterpret_cast<const char *>(&_data), sizeof(data));
        for (int j = proper_pos; j < head_list[block_num].size; ++j) {
            iof.write(reinterpret_cast<char *>(temp + j), sizeof(data));
        }
        if (_data < head_list[block_num].minimum) head_list[block_num].minimum = _data;
        if (head_list[block_num].maximum < _data) head_list[block_num].maximum = _data;
    }
    head_list[block_num].size++;
    delete[]temp;
}

void unroll_link::EraseInArray(long block_num, int position, data *temp) {
    iof.seekp(head_list[block_num].pos + position * sizeof(data));
    for (int i = position + 1; i < head_list[block_num].size; ++i) {
        iof.write(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    if (head_list[block_num].size == 1) {  //如果删到了最后一个块
        head_list.erase(head_list.begin() + block_num);
        block_size--;
        delete[]temp;
        recycle_bin.push_back(head_list[block_num].pos);
        return;
    }
    if (position == 0) head_list[block_num].minimum = temp[1]; //更新最小值
    if (position == head_list[block_num].size - 1) {  //更新最大值
        head_list[block_num].maximum = temp[head_list[block_num].size - 2];
    }
    delete[] temp;
    --head_list[block_num].size;
    if (head_list[block_num].size >= minsize) return;
    if (block_num != block_size - 1 && head_list[block_num + 1].size > minsize) {//向下一个块借元素
        data *temp1 = new data[head_list[block_num + 1].size];
        iof.seekg(head_list[block_num + 1].pos);
        for (int i = 0; i < head_list[block_num + 1].size; ++i) {
            iof.read(reinterpret_cast<char *>(temp1 + i), sizeof(data));
        }
        iof.seekp(head_list[block_num].pos + head_list[block_num].size * sizeof(data));
        iof.write(reinterpret_cast<char *>(temp1), sizeof(data));
        iof.seekp(head_list[block_num + 1].pos);
        for (int i = 1; i < head_list[block_num + 1].size; ++i) {
            iof.write(reinterpret_cast<char *>(temp1 + i), sizeof(data));
        }
        head_list[block_num].maximum = temp1[0];
        head_list[block_num + 1].minimum = temp1[1];
        head_list[block_num].size++;
        head_list[block_num + 1].size--;
        delete[] temp1;
    } else if (block_num > 0 && head_list[block_num - 1].size > minsize) { //向上一个块借元素
        data *temp2 = new data[head_list[block_num].size + 1];
        data last_max{};
        iof.seekg(head_list[block_num - 1].pos + sizeof(data) * (head_list[block_num - 1].size - 2));
        iof.read(reinterpret_cast<char *>(&last_max), sizeof(data));
        iof.read(reinterpret_cast<char *>(temp2), sizeof(data));
        iof.seekg(head_list[block_num].pos);
        for (int i = 0; i < head_list[block_num].size; ++i) {
            iof.read(reinterpret_cast<char *>(temp2 + i + 1), sizeof(data));
        }
        iof.seekp(head_list[block_num].pos);
        for (int i = 0; i < head_list[block_num].size + 1; ++i) {
            iof.write(reinterpret_cast<char *>(temp2 + i), sizeof(data));
        }
        head_list[block_num].minimum = temp2[0];
        head_list[block_num - 1].maximum = last_max;
        head_list[block_num].size++;
        head_list[block_num - 1].size--;
        delete[] temp2;
    } else if (block_num != 0 && head_list[block_num - 1].size == minsize) {
        this->BlockMerge(block_num - 1);
    } else if (block_num != block_size - 1 && head_list[block_num + 1].size == minsize) {
        this->BlockMerge(block_num);
    }
}


void unroll_link::InsertInBlock(const char *index, const int &value) {
    data temp_data(index, value);
    if (block_size == 0) {
        block_node new_node(0, 1);
        new_node.maximum = temp_data;  //注意，这里是调用了重载赋值运算符，不是复制构造函数
        new_node.minimum = temp_data;
        block_size = 1;
        head_list.push_back(new_node);
        iof.seekp(0);
        iof.write(reinterpret_cast<char *>(&temp_data), sizeof(data));
    } else {
        int block_num = FindBlockNumIns(temp_data);
        this->InsertInArray(temp_data, block_num);
        if (head_list[block_num].size == maxsize) this->BlockSplit(block_num);
    }
}

int unroll_link::FindBlockNumIns(const data &x) {
    if (block_size == 1) {
        return 0;
    }
    for (int i = 0; i < block_size; ++i) {
        if (x < head_list[i].maximum) return i;
    }
    return block_size - 1;
}

std::vector<int> unroll_link::FindInArray(const std::vector<int> &block_num_vec, const char *index) {
    std::vector<int> ans;
    for (int it: block_num_vec) {
        iof.seekg(head_list[it].pos);
        data temp{};
        for (int i = 0; i < head_list[it].size; ++i) {
            iof.read(reinterpret_cast<char *>(&temp), sizeof(data));
            if (strcmp(temp.index, index) == 0) {
                ans.push_back(temp.value);
            }
        }
    }
    return ans;
}
    void unroll_link::EraseInBlock(const char *index, const int &value) {
        data temp_data(index, value);
        long block_num = FindBlockNum(temp_data);  //这是要删掉元素的块位置
        if (block_num == -1) return;
        int pos = -1;   //这是在块中要被删掉的元素位置
        iof.seekg(head_list[block_num].pos);
        data *temp = new data[head_list[block_num].size];
        for (int i = 0; i < head_list[block_num].size; ++i) {
            iof.read(reinterpret_cast<char *>(temp + i), sizeof(data));
            if (temp[i] == temp_data) pos = i;
        }
        if (pos == -1) {
            delete[]temp;
            return;
        }
        this->EraseInArray(block_num, pos, temp); //为了减少文件读写的次数，直接把temp数组指针传出去
    }

    std::string unroll_link::FindInBlock(const char *_index) {
        std::string ans;
        std::vector<int> block_num_vec = this->FindBlockNumFind(_index);
        if (block_num_vec.empty()) return "null\n";
        std::vector<int> value_vec = this->FindInArray(block_num_vec, _index);
        if (value_vec.empty()) return "null\n";
        for (const int &it: value_vec) {
            ans += std::to_string(it);
            ans += " ";
        }
        ans += "\n";
//        ans = "   " + ans;
//        ans = _index + ans;
        return ans;
    }

    unroll_link::unroll_link(std::string
    name, std::string
    other_name) : filename(std::move(name)),
            other_information_file(std::move(other_name))
    {

        std::ifstream in1(filename), in2(other_information_file);
        if (!in2) {//如果没有打开成功，说明是初次使用系统，就创建这两个文件
            std::ofstream out1(filename), out2(other_information_file);
            block_size = 0;
            recycle_size = 0;
            out2.write(reinterpret_cast<char *>(&block_size), sizeof(int));
            out2.write(reinterpret_cast<char *>(&recycle_size), sizeof(int));
            out1.close();
            out2.close();
            iof.open(filename, std::fstream::in | std::fstream::out);
            return;

        } else {
            in1.close();
            iof.open(filename, std::fstream::in | std::fstream::out);
            recycle_bin.clear();
            in2.read(reinterpret_cast<char *>(&block_size), sizeof(int));//读入块的总数
            for (int i = 0; i < block_size; ++i) {
                block_node temp{};
                in2.read(reinterpret_cast<char *>(&temp), sizeof(block_node));
                head_list.push_back(temp);
            }
            in2.read(reinterpret_cast<char *>(&recycle_size), sizeof(int));
            long temp;
            for (int i = 0; i < recycle_size; ++i) {
                in2.read(reinterpret_cast<char *>(&temp), sizeof(long));
                recycle_bin.push_back(temp);
            }
            in2.close();
        }
    }

    unroll_link::~unroll_link()
    {
        std::ofstream out2(other_information_file);//用ofstream打开的时候会默认清空！
        out2.write(reinterpret_cast<char *>(&block_size), sizeof(int));
        for (int i = 0; i < block_size; ++i) {
            out2.write(reinterpret_cast<char *>(&head_list[i]), sizeof(block_node));
        }
        recycle_size = (int)recycle_bin.size();
        out2.write(reinterpret_cast<char *>(&recycle_size), sizeof(int));
        for (long & i : recycle_bin) {
            out2.write(reinterpret_cast<char *>(&i), sizeof(long));
        }
        out2.close();
        iof.close();
    }

    long unroll_link::FindBlockNum(const data &_data) {
        if (block_size == 0) {
            return -1;
        }
        for (int i = 0; i < head_list.size(); ++i) {
            if (head_list[i].minimum <= _data && _data <= head_list[i].maximum) {
                return i;
            }
        }
        return -1;
    }

    std::vector<int> unroll_link::FindBlockNumFind(const char *_index) {
        std::vector<int> ans;
        for (int i = 0; i < head_list.size(); ++i) {
            if (strcmp(head_list[i].minimum.index, _index) <= 0 && strcmp(head_list[i].maximum.index, _index) >= 0)
                ans.push_back(i);
        }
        return ans;
    }






