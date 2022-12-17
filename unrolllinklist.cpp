#include "unrolllinklist.h"

#include <utility>

std::vector<int> recycle_bin; //储存现在是空的的位置

//std::set<block_node> block_info;//用于储存节点信息

char *min(char *a, char *b) {
    return strcmp(a, b) < 0 ? a : b;
}

char *max(char *a, char *b) {
    return strcmp(a, b) > 0 ? a : b;
}


long to_block_pos(int pos){
    return sizeof(int)+pos*sizeof (block_node);
}

long to_array_pos(int pos){
    return (pos-2)*maxsize*sizeof (data);
}

int array_pos_to_pos(long array_pos){
    return array_pos/sizeof (data)+2;
}

int block_pos_to_pos(long block_pos){
    return (block_pos-sizeof(int))/sizeof(block_node);
}


data::data(const char *_index, int _value) : value(_value) {
    strcpy(index, _index);
}

block_node::block_node(long _next, long _last, int _pos) : next(_next), last(_last),pos(_pos){
    size = 1;
    strcpy(maximum, "");
    strcpy(minimum, "");
}

block_node::~block_node() { //把这个地址放到回收站里 注意，在这里没法操作一个块状链表的快数
}

void block_node::BlockMerge(block_node *y, const unroll_link &_unrollLink) {
    this->next = y->next;
    y->next->last = this;

    data *temp = new data[y->size]; //进行处理：数据迁移
    std::ifstream in;
    std::ofstream out;
    in.open(_unrollLink.);
    in.seekg(y->pos);
    for (int i = 0; i < y->size; ++i) {  //把y中的数据读到数组temp里
        in.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    in.close();
    out.open(_unrollLink.filename);
    out.seekp(pos + size);
    for (int i = 0; i < y->size; ++i) {
        out.write(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    out.close();
    delete[]temp;
    this->size += y->size;
    strcpy(this->maximum, y->maximum);
    delete y;
}

void block_node::BlockSplit(unroll_link &_unrollLink) {  //在this的后面新插入一个块,给this留下minsize个元素，从minsize-size个元素赋给y
    int y_pos; //确定y的数组的起始位置
    if (!recycle_bin.empty()) {
        y_pos = recycle_bin.back();
        recycle_bin.pop_back();
    } else {
        std::ifstream in(_unrollLink.filename);
        in.seekg(sizeof(data) * _unrollLink.block_size);
        _unrollLink.block_size++;
        y_pos = array_pos_to_pos(in.tellg());
        in.close();
    }
    block_node y(next, to_block_pos(pos),y_pos); //需要新插入的块
    std::ifstream in1(_unrollLink.filename);
    block_node last_node,next_node;
    in1.seekg(last);
    in1.read(reinterpret_cast<char *>(&last_node),sizeof (block_node));
    in1.seekg(next);
    in1.read(reinterpret_cast<char *>(&next_node),sizeof (block_node));
    next_node.last = to_block_pos(y_pos);
    next = to_array_pos(y_pos);
    y.size = size - minsize;
    data this_max, y_min;
    std::ifstream in(_unrollLink.filename);
    in.seekg(to_array_pos(pos) + minsize * sizeof(data));
    data *temp = new data[size - minsize];  //用temp承接需要被转移的数据
    for (int i = 0; i < size - minsize; ++i) {
        in.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    in.seekg(to_array_pos(pos) + (minsize - 1) * sizeof(data));
    in.read(reinterpret_cast<char *>(&this_max), sizeof(data)); //读入当前数组的新最大值
    in.read(reinterpret_cast<char *>(&y_min), sizeof(data));  //读入下一个数组的最小值
    in.close();
    std::ofstream out(_unrollLink.filename);
    out.seekp(to_array_pos(y.pos));
    for (int i = 0; i < y.size; ++i) {
        out.write(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    out.close();
    delete[]temp;
    strcpy(y.maximum, maximum);
    strcpy(maximum, this_max.index);
    strcpy(y.minimum, y_min.index);
    size = minsize;
    std::ofstream out1(_unrollLink.node_filename);
    out1.seekp(to_block_pos(last_node.pos));
    out1.write(reinterpret_cast<char *>(&last_node),sizeof (block_node));
    out1.seekp(to_block_pos(pos));
    out1.write(reinterpret_cast<char *>(this),sizeof(block_node));
    out1.seekp(to_block_pos(next_node.pos));
    out1.write(reinterpret_cast<char *>(&next_node),sizeof(block_node));
    out1.seekp(to_block_pos(y.pos));
    out1.write(reinterpret_cast<char *>(&y),sizeof(block_node));
    out1.close();
}

void block_node::InsertInArray(const data &_data, const unroll_link &unrollLink) { //无需考虑是否会裂块!
    data *temp = new data[size];
    std::ifstream in(unrollLink.filename);
    in.seekg(to_array_pos(pos));
    for (int i = 0; i < size; ++i) {
        in.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    in.close();

    if (strcmp(_data.index, minimum) <= 0) { //在头端插入
        std::ofstream out(unrollLink.filename);
        out.seekp(to_array_pos(pos));
        out.write(reinterpret_cast<const char *>(&_data),
                  sizeof(data));   //因为传进来的_data是一个const &,所以不能解释成一个char *,应该解释成一个const char *
        for (int i = 0; i < size; ++i) {
            out.write(reinterpret_cast<char *>(temp + i), sizeof(data));
        }
        out.close();
        delete[] temp;
        ++size;
        strcpy(minimum, _data.index);
        std::ofstream out1(unrollLink.node_filename);
        out1.seekp(to_block_pos(pos));
        out1.write(reinterpret_cast<char *>(this),sizeof (block_node));
        out1.close();
        return;
    }
    if (strcmp(_data.index, maximum) >= 0) { //在尾部插入
        std::ofstream out(unrollLink.filename);
        out.seekp(to_array_pos(pos) + sizeof(data) * size);
        out.write(reinterpret_cast<const char *>(&_data), sizeof(data));
        out.close();
        delete[] temp;
        ++size;
        strcpy(maximum, _data.index);
        std::ofstream out1(unrollLink.node_filename);
        out1.seekp(to_block_pos(pos));
        out1.write(reinterpret_cast<char *>(this),sizeof (block_node));
        out1.close();
        return;
    }

    int ii = 0;
    for (int i = 0; i < size; ++i) {
        if (strcmp(temp[i].index, _data.index) <= 0 && strcmp(temp[i + 1].index, _data.index) >= 0) {
            ii = i;
            break;
        }
    }
    std::ofstream out(unrollLink.filename);
    out.seekp(to_array_pos(pos) + (ii + 1) * sizeof(data));
    out.write(reinterpret_cast<const char *>(&_data), sizeof(data));
    for (int i = ii + 1; i < size; ++i) {
        out.write(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    out.close();
    delete[] temp;
    std::ofstream out1(unrollLink.node_filename);
    out1.seekp(to_block_pos(pos));
    out1.write(reinterpret_cast<char *>(this),sizeof (block_node));
    out1.close();
    ++size;
}

void block_node::EraseInArray(int position, data *temp_data, unroll_link &_unrollLink) {  //data里存了所有当前数组的数据
    --size;
    std::ofstream out(_unrollLink.filename),out1(_unrollLink.node_filename);
    block_node last_node,next_node;
    out.seekp(to_array_pos(pos)+position*sizeof (data));
    out1.seekp(last);
    out1.write(reinterpret_cast<char *>(&last_node),sizeof (block_node));
    out1.seekp(next);
    out1.write(reinterpret_cast<char *>(&next_node),sizeof (block_node));
    for (int i = position + 1; i < size; ++i) {
        out.write(reinterpret_cast<char *>(temp_data + i), sizeof(data));
    }
    out.close();
    if (size == 0) {  //删到最后，直接把这个块给删掉
        _unrollLink.block_size--;
        last_node.next=next;
        next_node.last=last;
        recycle_bin.push_back(pos);
    }else {
        if (position == 0) strcpy(minimum, temp_data[1].index);
        if (position == size - 1) strcpy(maximum, temp_data[size - 2].index);
        delete[] temp_data;
        if (size < minsize) {
            if (next != to_block_pos(_unrollLink.tail.pos) && next_node.size > minsize) {//向下一个块借元素
                data new_data;
                std::ifstream in3(_unrollLink.filename);
                in3.seekg(next);
                in3.read(reinterpret_cast<char *>(&new_data), sizeof(data));
                data temp[next_node.size-1];  //把下一个数组的剩余数据读进来，准备给下一个数组的数据集体向前移
                for(int i=0;i<next_node.size-1;++i) in3.read(reinterpret_cast<char *>(temp+i),sizeof (data));
                std::ofstream out3(_unrollLink.filename);
                out3.seekp(to_array_pos(pos) + size * sizeof(data));
                out3.write(reinterpret_cast<char *>(&new_data), sizeof(data));
                out3.seekp(next);
                for(int i=0;i<next_node.size-1;++i) out3.write(reinterpret_cast<char *>(temp+i),sizeof (data));
                strcpy(maximum, new_data.index);
                strcpy(next_node.minimum,temp[0].index);
                size++;
                next_node.size--;
                out3.close();
            } else if (last != to_block_pos(_unrollLink.head.pos) && last_node.size> minsize) { //向上一个块借元素
                data temp[size+1],last_max;
                std::ifstream in2(_unrollLink.filename);
                in2.seekg(last+sizeof (data)*(last_node.size-2));
                in2.read(reinterpret_cast<char *>(&last_max),sizeof (data));
                in2.read(reinterpret_cast<char *>(temp), sizeof(data));
                in2.seekg(to_array_pos(pos));
                for (int i = 0; i < size; ++i) {
                    in2.read(reinterpret_cast<char *>(temp + i + 1), sizeof(data));
                }
                in2.close();
                std::ofstream out2(_unrollLink.filename);
                out2.seekp(to_array_pos(pos));
                for (int i = 0; i < size+1; ++i) {
                    out2.write(reinterpret_cast<char *>(temp + i), sizeof(data));
                }
                strcpy(minimum, temp[0].index);
                strcpy(last_node.maximum,last_max.index);
                size++;
                last_node.size--;
                out2.close();
            } else if (last != to_block_pos(_unrollLink.head.pos) && last_node.size == minsize) {
                last_node.BlockMerge(this, _unrollLink);
            } else if (next != to_block_pos(_unrollLink.tail.pos) && next_node.size == minsize) {
                this->BlockMerge(&next_node, _unrollLink);
            }
        }
        std::ofstream out3(_unrollLink.node_filename);
        out3.seekp(last);
        out3.write(reinterpret_cast<char *>(&next_node),sizeof (data));
        out3.seekp(to_block_pos(pos));
        out3.write(reinterpret_cast<char *>(this),sizeof(data));
        out3.seekp(next);
        out3.write(reinterpret_cast<char *>(&last_node),sizeof (data));
    }
}


void unroll_link::InsertInBlock(const char *index, const int &value) {
    char _index[70];
    strcpy(_index, index);
    data temp(_index, value);
    if (block_size==0) {
        block_node node(to_block_pos(tail.pos), to_block_pos(head.pos),2);
        head.next= to_block_pos(node.pos);
        tail.last= to_block_pos(node.pos);
        block_size = 1;
        strcpy(node.minimum, _index);
        strcpy(node.maximum, _index);
        node.size = 1;
        std::ofstream out;
        out.open(filename);
        out.write(reinterpret_cast<char *>(&temp), sizeof(data));
        out.close();
    } else {
        int try_result = TryFind(_index);
        std::ifstream in(node_filename);
        block_node try_result_block;
        in.seekg(to_block_pos(try_result));
        in.read(reinterpret_cast<char *>(&try_result_block),sizeof (block_node));
        in.close();
        try_result_block.InsertInArray(temp, *this);
        if (try_result_block.size == maxsize) try_result_block.BlockSplit(*this);
    }
}

int unroll_link::TryFind(const char *index) {
    std::ifstream in(node_filename);
    long temp=head.next;
    while(temp!= to_block_pos(tail.pos)){
        block_node temp_block;
        in.seekg(temp);
        in.read(reinterpret_cast<char *>(&temp_block),sizeof(block_size));
        if (strcmp(index, temp_block.minimum) >= 0 && strcmp(index, temp_block.maximum) <= 0) {
            in.close();
            return block_pos_to_pos(temp);
        }
        temp=temp_block.next;
    }
    block_node head_next,tail_last;
    in.seekg(head.next);
    in.read(reinterpret_cast<char *>(&head_next),sizeof(block_node));
    in.seekg(tail.last);
    in.read(reinterpret_cast<char *>(&tail_last),sizeof(block_node));
    in.close();
    if (strcmp(index, head_next.minimum) < 0) { //应该插在第一个array的开头
        return block_pos_to_pos(head.next);
    }
    return block_pos_to_pos(tail.last);  //应该插在最后一个array的末尾
}

std::vector<int> block_node::TryFindInArray(const char *index, unroll_link &unrollLink) const {
    std::vector<int> ans;
    std::ifstream in(unrollLink.filename);
    in.seekg(pos);
    data *temp = new data[size];
    for (int i = 0; i < size; ++i) {
        in.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    in.close();
    bool if_find = false;
    for (int i = 0; i < this->size; ++i) {
        if (strcmp(temp[i].index, index) == 0) {
            if_find = true;
            ans.push_back(temp[i].value);
        }
    }
    delete[]temp;
    if (!if_find) ans.push_back(-1);
    return ans;
}

void unroll_link::EraseInBlock(const char *index, const int &value) {
    if(block_size==0) return;
    int try_result = TryFind(index);
    std::ifstream in(filename),in1(node_filename);
    in.seekg(to_array_pos(try_result));
    in1.seekg(to_block_pos(try_result));
    block_node temp_block; //这是需要被删数据的块
    in1.read(reinterpret_cast<char *>(&temp_block),sizeof (block_node));
    in1.close();
    data *temp = new data[temp_block.size];
    for (int i = 0; i < temp_block.size; ++i) {
        in.read(reinterpret_cast<char *>(temp + i), sizeof(data));
    }
    in.close();
    int pos = -1;
    for (int i = 0; i < temp_block.size; ++i) {
        if (strcmp(temp[i].index, index) == 0 && temp[i].value == value) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        delete[]temp;
        return;
    }
    temp_block.EraseInArray(pos, temp, *this); //data里面存着所有的当前数组的数据
}

std::string unroll_link::FindInBlock(const char *_index) {
    std::string ans;
    block_node *try_result = this->TryFind(_index);
    std::vector<int> position = try_result->TryFindInArray(_index, *this);
    if (position[0] == -1) return "null\n";
    for (int &it: position) {
        ans += std::to_string(it);
        ans += " ";
    }
    ans += "\n";
    return ans;
}

unroll_link::unroll_link(std::string name, std::string node_name, const std::string& recycle_name) : filename(std::move(name)),
                                                                                              node_filename(std::move(node_name)),
                                                                                              recycle_filename(
                                                                                                      recycle_name) {
    std::ifstream in1(filename), in2(node_filename), in3(recycle_filename);
    if (!in1) {  //如果没有打开成功，说明是初次使用系统，就创建这三个文件
        std::ofstream out1(filename), out2(node_filename), out3(recycle_filename);
        out1.close();
        block_size = 0;
        out2.write(reinterpret_cast<char *>(&block_size), sizeof(int));
        head=block_node(sizeof(int)+sizeof (block_node),-1,0);
        tail=block_node(-2,sizeof (int),1);
        out2.write(reinterpret_cast<char *>(&head),sizeof(block_size));//写入头结点
        out2.write(reinterpret_cast<char *>(&tail),sizeof(block_size));//写入尾结点
        out2.close();
        recycle_size = 0;
        out3.write(reinterpret_cast<char *>(&recycle_size), sizeof(int));
        out3.close();
        return;
    }
    in2.read(reinterpret_cast<char *>(&block_size), sizeof(int));
    in2.read(reinterpret_cast<char *>(&head),sizeof(block_size));
    in2.read(reinterpret_cast<char *>(&tail),sizeof (block_size));

    in3.read(reinterpret_cast<char *>(&recycle_size), sizeof(int));
    for (int i = 1; i <= recycle_size; ++i) {
        int temp;
        in3.read(reinterpret_cast<char *>(temp), sizeof(int));
        recycle_bin.push_back(temp);
    }
    in1.close();
    in2.close();
    in3.close();
}

unroll_link::~unroll_link() {
    std::ofstream out2(node_filename),out3(recycle_filename);
    out2.write(reinterpret_cast<char *>(&block_size),sizeof (int));
    out2.write(reinterpret_cast<char *>(&head),sizeof(block_node));
    out2.write(reinterpret_cast<char *>(&tail),sizeof(block_node));
    out2.close();
    out3.write(reinterpret_cast<char *>(&recycle_size), sizeof(int));
    for(auto & it:recycle_bin){
        out3.write(reinterpret_cast<char *>(&it),sizeof (int));
    }
    out3.close();
}



