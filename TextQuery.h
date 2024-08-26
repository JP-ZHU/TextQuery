#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include<memory>
#include<vector>
#include<set>
#include<string>
#include<map>
#include<fstream>
#include<sstream>

#include "Query_base.h"

using line_no=std::vector<std::string>::size_type;//定义类型别名方便代码书写
class QueryResult;

class TextQuery
{
private:
    std::shared_ptr<std::vector<std::string>> file;//输入文件
    std::map<std::string,std::shared_ptr<std::set<line_no>> >wm;//单词与指向存储所在行号的set的智能指针映射
public:
    TextQuery(std::ifstream&);//构造函数，接受一个文件
    QueryResult query(const std::string& s)const;//s应该为整个查询子句
    ~TextQuery();
};

#endif