/**
 * @description: 
 * @return {*}
 */
#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include<memory>
#include<vector>
#include<set>
#include<string>
#include<map>
#include<fstream>
#include<sstream>

using line_no=std::vector<std::string>::size_type;//定义行号类型别名

class QueryResult;

/**
 * @description: 
 * 接受一个文本文件的输入，用vector逐行保存文本，用map实现单词与行号的映射
 * 提供查询接口函数
 * @return {*}
 */
class TextQuery
{
private:
    std::shared_ptr<std::vector<std::string>> file;//输入文本文件按行存储
    std::map<std::string,std::shared_ptr<std::set<line_no>> >wm;//某个单词与存储行号的集合的映射
public:
    TextQuery(std::ifstream&);
    QueryResult query(const std::string& s)const;//单个单词查询
    QueryResult query_clause(const std::string& s)const;//子句查询
    ~TextQuery()=default;
};

#endif