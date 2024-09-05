#include<stack>
#include<string>
#include<iostream>
#include<cctype>
#include "Query_base.h"

class Query;

//处理字符串，只保留字母，去除标点符号
std::string extractWord(const std::string& text){
    std::string cleanText;
    for(char c:text){
        if(isalpha(c)){
            cleanText+=c;
        }  
    }
    return cleanText;
}

//处理子句，返回最外层逻辑运算符的位置
int split(const std::string& s){
    std::stack<char> stk;
    int idx=1;
    while(idx!=s.size()-1){
        if((s[idx]=='&'||s[idx]=='|')&&stk.empty()){
            std::cout<<"split func exeted with output"<<idx<<std::endl;
            std::cout<<"$$"<<s[idx]<<"$$"<<std::endl;
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

    Query QueryTree(const std::string& s){
         //std::cout << "Input string length: " << s.length() << std::endl;
         //std::cout << "Input string: " << s << std::endl;
        //情况1：括号开头，调用二元逻辑运算
        if(s[0]=='('){
            std::cout << "Checking if starts with ( - character is: " << s[0] << std::endl;
            std::cout<<"situation 1 executed"<<std::endl;
            int pos=split(s);
            std::cout<<"the result of split func is:"<<pos<<std::endl;
            std::string ls,rs;
            ls=s.substr(1,pos-1);
            rs=s.substr(pos+1,s.size()-2-pos);
            if(s[pos]=='&'){
                std::cout << "$$Building AndQuery with left: " << ls << " and right: " << rs << std::endl;
                return QueryTree(ls)&QueryTree(rs);
            }
            else if(s[pos]=='|'){
                std::cout << "$$Building OrQuery with left: " << ls << " and right: " << rs << std::endl;
                return QueryTree(ls)|QueryTree(rs);
            }
        }
        //情况2：~开头，调用notquery
        if(s[0]=='~'){
            std::cout<<"situation 2 executed"<<std::endl;
            std::string ls=s.substr(2,s.size()-1-2);
            std::cout << "$$Building NotQuery with left: " << ls << std::endl;
            return ~QueryTree(ls);
        }
        //情况3：只含有单词，不含任何括号，运算符，调用wordquery
        else{
            std::cout<<"situation 3 executed"<<std::endl;
            std::cout << "$$Building WordQuery with clause: " << s << std::endl;
            return Query(s);
        }
    }
