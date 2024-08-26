#include<algorithm>
#include<stack>
#include "Query_base.h"
//输出运算符存疑
std::ostream& operator<<(std::ostream& os,const Query& query){
    return os<<query.rep();
}

Query Query::BuildTree(const std::string& s){
    std::stack<std::string> stk;
    //情况1：只含有单词，不含任何括号，运算符
    if(s[0]!='('&&s[0]!='~'){
        return Query(s);
    }
    if(s[0]=='('){
        std::string ls,rs;
        std::string::iterator oprand=
    }
}

QueryResult OrQuery::eval(const TextQuery& text)const{
    auto right=rhs.eval(text),left=lhs.eval(text);//右边和左边的query对象分别调用其eval函数，返回类型均为QueryResult
    auto ret_lines=std::make_shared<std::set<line_no>>(left.begin(),left.end());
    ret_lines->insert(right.begin(),right.end());
    return QueryResult(rep(),ret_lines,left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text)const{
    auto left=lhs.eval(text),right=rhs.eval(text);
    auto ret_lines=std::make_shared<std::set<line_no>>();
    std::set_intersection(left.begin(),left.end(),right.begin(),right.end(),std::inserter(*ret_lines,ret_lines->begin()));
    return QueryResult(rep(),ret_lines,left.get_file()); 
}

QueryResult NotQuery::eval(const TextQuery& text)const{
    auto result=query.eval(text);
    auto ret_lines=std::make_shared<std::set<line_no>>();
    auto beg=result.begin(),end=result.end();
    auto sz=result.get_file()->size();
    for(size_t n=0;n!=sz;++n){
        if(beg==end||*beg!=n){
            ret_lines->insert(n);
        }
        else if(beg!=end){
            ++beg;
        }
    }
    return QueryResult(rep(),ret_lines,result.get_file());
}