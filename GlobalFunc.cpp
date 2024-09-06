/**
 * @description: 全局函数文件
 */
#include<stack>
#include<string>
#include<iostream>
#include<cctype>

#include "Query_base.h"

class Query;

/**
 * @description: 
 * 处理字符串，去除标点符号只保留字母
 */
std::string extractWord(const std::string& text){
    std::string cleanText;
    for(char c:text){
        if(isalpha(c)){
            cleanText+=c;
        }  
    }
    return cleanText;
}

/**
 * @description: 
 * 栈括号匹配实现查找最外层逻辑运算符
 * @param {string&} s 子查询对象
 * @return {*} 返回逻辑运算符所在位次
 */
int extractSymbol(const std::string& s){
    std::stack<char> stk;
    int idx=1;
    while(idx!=s.size()-1){
        if((s[idx]=='&'||s[idx]=='|')&&stk.empty()){
            return idx;
        }
        else if(s[idx]==')'){
            stk.pop();
            ++idx;
        }
        else if(s[idx]=='('){
            stk.push(s[idx]);
            ++idx;
        }
        else ++idx;
    }
    return -1;
}

/**
 * @description: 
 * 递归构造Query对象查询树
 * @param {string&} s 输入的查询子句
 * @return {*} 返回Query对象
 */
Query QueryTree(const std::string& s){
    //情况1：子句为括号开头，调用二元逻辑运算
    if(s[0]=='('){
        int pos=extractSymbol(s);
        std::string ls,rs;
        ls=s.substr(1,pos-1);
        rs=s.substr(pos+1,s.size()-2-pos);
        if(s[pos]=='&'){
            return QueryTree(ls)&QueryTree(rs);
        }
        else if(s[pos]=='|'){
            return QueryTree(ls)|QueryTree(rs);
        }
    }
    //情况2：子句~开头，调用notquery
    if(s[0]=='~'){
        std::string ls=s.substr(2,s.size()-1-2);
        return ~QueryTree(ls);
    }
    //情况3：只含有单词，不含任何括号，运算符
    else{
        return Query(s);
    }
}
