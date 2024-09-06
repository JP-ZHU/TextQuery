#include<algorithm>
#include<stack>

#include "Query_base.h"
#include "QueryResult.h"
#include "TextQuery.h"

/**
 * @description: 
 * 或逻辑运算，对左右的Query对象分别调用eval函数，利用set的insert函数实现取并集
 * @param {TextQuery&} text 查询的文本对象
 * @return {*} 返回结果对象
 */
QueryResult OrQuery::eval(const TextQuery& text)const{
    auto right=rhs.eval(text),left=lhs.eval(text);//右边和左边的query对象分别调用其eval函数，返回类型均为QueryResult
    auto ret_lines=std::make_shared<std::set<line_no>>(left.begin(),left.end());
    ret_lines->insert(right.begin(),right.end());
    return QueryResult(rep(),ret_lines,left.get_file());
}

/**
 * @description: 
 * 与逻辑运算，对左右的Query对象分别调用eval函数，利用set_interseciton函数实现取交集
 * @param {TextQuery&} text 查询的文本对象
 * @return {*} 返回结果对象
 */
QueryResult AndQuery::eval(const TextQuery& text)const{
    auto left=lhs.eval(text),right=rhs.eval(text);
    auto ret_lines=std::make_shared<std::set<line_no>>();
    std::set_intersection(left.begin(),left.end(),right.begin(),right.end(),std::inserter(*ret_lines,ret_lines->begin()));
    return QueryResult(rep(),ret_lines,left.get_file()); 
}
/**
 * @description: 
 * 非逻辑运算，根据一般的query查询返回结果，重新遍历返回非结果
 * @param {TextQuery&} text 查询的文本对象
 * @return {*} 返回结果对象
 */
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