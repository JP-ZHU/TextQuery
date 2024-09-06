/*
 * @Author: zhu 
 * @Date: 2024-08-28 15:44:59
 * @LastEditors: zhu 3180104456@zju.edu.cn
 * @LastEditTime: 2024-09-06 17:49:07
 * @FilePath: \TextQuery\main.cpp
 * @Description: 
 * 实现了给定文本文件，查询若干单词所在行的功能
 * 技术特点：类继承特性、虚函数、字符串处理、STL、智能指针
 * 查询子句语法：
 * "word":输出含有字符串word的行
 * ~("word")：查询并输出所有不含字符串word的行
 * ("word_1"&"word_2"):输出含有word_1和word_2的行
 * ("word_1"|"word_2"):输出含有word_1或word_2的行
 * (("word_1"&"word_2")|"word_3"):输出含有word_1和word_2的行或者仅含有word_3的行
 */

#include<iostream>
#include<string>
#include<fstream>

#include "TextQuery.h"
#include "QueryResult.h"

/**
 * @description: 在给定流上打印出给定对象的查询结果
 * @param {ostream&} os 输出流
 * @param {QueryResult} &qr 查询返回的结果对象
 * @return {*}
 */
std::ostream &print(std::ostream& os,const QueryResult &qr){
    auto plural=qr.lines->size()>1?"times":"time";
    os<<qr.sought<<" occuers "<<qr.lines->size()<<" "<<plural<<std::endl;
    for(auto num: *qr.lines){
        os<<"\t(line"<<num+1<<")"<<*(qr.file->begin()+num)<<std::endl;
    }
    return os;
}

/**
 * @description: 查询功能的实现主体，while循环提示查询对象，并打印查询结果
 * @param {ifstream} &infile 输入的查询文本文件
 * @return {*}
 */
void runQueries(std::ifstream &infile){
    //扫描文件，保存结果
    TextQuery tq(infile);
    //输入指定单词查询
    while(1){
        std::cout<<"enter expression to look for,or q to quit:";
        std::string s;
        if(!(std::cin>>s)||s=="q"){
            break;
        }
        std::cout<<"Exeuting Query for:"<<s<<std::endl;
        print(std::cout,tq.query_clause(s))<<std::endl;
    }
}

int main(){
    std::string filename;
    std::cout<<"Please enter the name of the .txt file to load:";
    std::cin>>filename;
    std::ifstream infile(filename);
    //找不到指定文件抛出异常
    if(!infile){
        std::cerr<<"Could not open the file:"<<filename<<std::endl;
        return 1;
    }
    runQueries(infile);
    infile.close();
    return 0;
}