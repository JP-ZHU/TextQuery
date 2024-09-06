#include<iostream>
#include "TextQuery.h"
#include "QueryResult.h"
#include "Query_base.h"

class QueryResult;
std::string extractWord(const std::string& text);//去除文本标点符号
Query QueryTree(const std::string& s);//构建查询对象树

/**
 * @description: 
 * TextQuery的构造函数，接受输入文本，逐行读取字符串，转化为单词，去除标点符号，建立映射，存储文本
 * @param {ifstream} &is 文件输入流
 */
TextQuery::TextQuery(std::ifstream &is):file(new std::vector<std::string>)
{   
    std::string text;
    while(getline(is,text)){
        file->push_back(text);
        int n=file->size()-1;
        std::istringstream line(text);
        std::string word;
        while(line>>word){
            word=extractWord(word);
            auto &lines=wm[word];
            if(!lines){
                lines.reset(new std::set<line_no>);
            }
            lines->insert(n);
        }
    }
}

/**
 * @description: 
 * 接受查询子句，构建查询对象树
 * @param {string&} s 查询子句
 * @return {*} 返回QueryResult类的查询结果
 */
QueryResult TextQuery::query_clause(const std::string& s)const{
    Query q=QueryTree(s);
    return q.eval(*this);
}

/**
 * @description: 
 * 接受单词，返回查询结果
 * @param {string} &sought
 * @return {*}
 */
QueryResult TextQuery::query(const std::string &sought)const{
    static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);//存储查找失败的结果
    auto loc=wm.find(sought);
    if(loc==wm.end()){
        return QueryResult(sought,nodata,file);
    }
    else{
        return QueryResult(sought,loc->second,file);
    }
}
