/**
 * @description: 
 * 定义继承树，包括抽象基类Query_Base，它的三个派生类WordQuery、NotQuery、BinaryQuery
 * BinaryQuery也是一个抽象基类，包含两个派生类AndQuery、OrQuery
 * 定义接口类Query封装Query_Base，提供逻辑与或非的查询功能
 */
#ifndef QUERY_BASE_H
#define QUERY_BASE_H

#include<string>

#include "QueryResult.h"

class TextQuery;
class QueryResult;

/**
 * @description: 
 * 抽象基类，构建查询层次关系
 */
class Query_base
{
    friend class Query;
private:
    virtual QueryResult eval(const TextQuery&) const=0;//返回匹配当前查询的查询结果
    virtual std::string rep()const=0;//返回查询的字符串表示形式
protected:
    virtual ~Query_base()=default;
};

/**
 * @description: 
 * 查找一个单词，是在给定的TextQuery对象上实际执行查询的操作
 */
class WordQuery:public Query_base
{
    friend class Query;
private:
    WordQuery(const std::string& s):query_word(s){}
    QueryResult eval(const TextQuery& t)const{return t.query(query_word);}
    std::string rep()const{return query_word;}
    std::string query_word;//要查找的单词
};

/**
 * @description: 
 * 接口类，隐藏继承结构
 */
class Query
{
    friend Query operator~(const Query&);
    friend Query operator|(const Query&,const Query&);
    friend Query operator&(const Query&,const Query&);
private:
    std::shared_ptr<Query_base> q;
    Query(std::shared_ptr<Query_base> query):q(query){}
public:
    Query QueryTree(const std::string&);//接受输入的查询子句，构建Query对象树
    Query(const std::string&);//构建wordquery对象
    QueryResult eval(const TextQuery &t)const{return q->eval(t);}//封装Query_Base类的eval
    std::string rep()const {return q->rep();}//封装Query_Base类的rep
    ~Query()=default;
};
inline Query::Query(const std::string& s):q(new WordQuery(s)){}

/**
 * @description: 
 * 取非逻辑查询类
 */
class NotQuery:public Query_base
{
private:
    friend Query operator~(const Query&);
    NotQuery(const Query& q):query(q){}
    std::string rep()const{return "~("+query.rep()+")";}
    QueryResult eval(const TextQuery&)const;
    Query query;
};
inline Query operator~(const Query& operand){
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}

/**
 * @description: 
 * 二元逻辑运算的抽象基类
 */
class BinaryQuery:public Query_base
{
protected:
    BinaryQuery(const Query& l,const Query& r,std::string s):lhs(l),rhs(r),opSym(s){}
    std::string rep()const{return "("+lhs.rep()+" "+opSym+" "+rhs.rep()+")";}
    Query lhs,rhs;//左子查询对象与右子查询对象
    std::string opSym;//二元逻辑操作符
};

/**
 * @description: 
 * 与逻辑查询类
 */
class AndQuery:public BinaryQuery
{
    friend Query operator&(const Query&,const Query&);
    AndQuery(const Query& left,const Query& right):BinaryQuery(left,right,"&"){}
    QueryResult eval(const TextQuery&)const;
};
inline Query operator&(const Query& lhs,const Query& rhs){
    return std::shared_ptr<Query_base>(new AndQuery(lhs,rhs));
}

/**
 * @description: 
 * 或逻辑查询类
 */
class OrQuery:public BinaryQuery
{
    friend Query operator|(const Query&,const Query&);
    OrQuery(const Query& left,const Query& right):BinaryQuery(left,right,"|"){}
    QueryResult eval(const TextQuery&)const;
};
inline Query operator|(const Query& lhs,const Query& rhs){
    return std::shared_ptr<Query_base>(new OrQuery(lhs,rhs));
}

#endif