#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include "TextQuery.h"
//保存文件扫描结果，输入查询结果
class QueryResult
{
    friend std::ostream& print(std::ostream&,const QueryResult&);
private:
    std::string sought;//待查询的单词
    std::shared_ptr<std::set<line_no>> lines;//出现的行号
    std::shared_ptr<std::vector<std::string>> file;//输入文件
public:
    QueryResult(std::string s,std::shared_ptr<std::set<line_no>> p,std::shared_ptr<std::vector<std::string>> f):
    sought(s),lines(p),file(f){}
    auto get_file(){return file;}
    auto begin()const->decltype(lines->begin()){
        return lines->begin();
    }
    auto end()const->decltype(lines->end()){
        return lines->end();
    }
    ~QueryResult();
};

#endif