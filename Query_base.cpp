#include<algorithm>
#include<stack>
#include "Query_base.h"
//输出运算符存疑
std::ostream& operator<<(std::ostream& os,const Query& query){
    return os<<query.rep();
}

//处理子句，返回最外层逻辑运算符的位置
int split(const std::string& s){
    std::stack<char> stk;
    int idx=1;
    while(idx!=s.size()-1){
        if((s[idx]=='&'||s[idx]=='|')&&stk.empty()){
            return idx;
        }
        if(s[idx]==')'){
            while(stk.top()!='('){
                stk.pop();
            }
            stk.pop();
        }
        else{
            stk.push(s[idx]);
        }
    }
    return -1;
}

Query Query::QueryTree(const std::string& s){
    //情况1：括号开头，调用二元逻辑运算
    if(s[0]=='('){
        int pos=split(s);
        std::string ls,rs;
        ls=s.substr(1,pos-1);
        rs=s.substr(pos+1,s.size()-2-pos);
        if(s[pos]=='&')
            return QueryTree(ls)&QueryTree(rs);
        else if(s[pos]=='|')
            return QueryTree(ls)|QueryTree(rs);
    }
    //情况2：~开头，调用notquery
    if(s[0]=='~'){
        std::string ls=s.substr(2,s.size()-1-2);
        return QueryTree(ls);
    }
    //情况3：只含有单词，不含任何括号，运算符，调用wordquery
    else{
        return Query(s);
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