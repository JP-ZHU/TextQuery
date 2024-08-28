#ifndef QUERY_BASE_H
#define QUERY_BASE_H

class TextQuery;

#include<string>
#include "QueryResult.h"
#include "TextQuery.h"

//抽象基类
class Query_base
{
    friend class Query;
private:
    virtual QueryResult eval(const TextQuery&) const=0;
    virtual std::string rep()const=0;
protected:
    virtual ~Query_base()=default;
};

class WordQuery:public Query_base
{
    friend class Query;
private:
    WordQuery(const std::string& s):query_word(s){}//wordquery构造函数，接受一个单词
    QueryResult eval(const TextQuery& t)const{return t.query(query_word);}
    std::string rep()const{return query_word;}
    std::string query_word;
};

//隐藏整个继承体系，为类用户提供接口
class Query
{
    friend Query operator~(const Query&);
    friend Query operator|(const Query&,const Query&);
    friend Query operator&(const Query&,const Query&);
private:
    std::shared_ptr<Query_base> q;
    Query(std::shared_ptr<Query_base> query):q(query){}

public:
    Query QueryTree(const std::string&);//接受输入的子句，返回一个query对象，构建对象树
    Query(const std::string&);//构建wordquery对象
    QueryResult eval(const TextQuery &t)const{return q->eval(t);}//
    std::string rep()const {return q->rep();}
    ~Query()=default;
};
inline Query::Query(const std::string& s):q(new WordQuery(s)){}



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

class BinaryQuery:public Query_base
{
protected:
    BinaryQuery(const Query& l,const Query& r,std::string s):lhs(l),rhs(r),opSym(s){}
    std::string rep()const{return "("+lhs.rep()+" "+opSym+" "+rhs.rep()+")";}
    Query lhs,rhs;
    std::string opSym;
};

class AndQuery:public BinaryQuery
{
    friend Query operator&(const Query&,const Query&);
    AndQuery(const Query& left,const Query& right):BinaryQuery(left,right,"&"){}
    QueryResult eval(const TextQuery&)const;
};
inline Query operator&(const Query& lhs,const Query& rhs){
    return std::shared_ptr<Query_base>(new AndQuery(lhs,rhs));
}

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