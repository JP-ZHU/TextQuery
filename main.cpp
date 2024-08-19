#include<iostream>
#include<string>
#include<fstream>

#include "TextQuery.h"
#include "QueryResult.h"


void runQueries(std::ifstream &infile){
    //扫描文件，保存结果
    TextQuery tq(infile);
    //输入指定单词查询
    while(1){
        std::cout<<"enter word to look for,or q to quit:";
        std::string s;
        if(!(std::cin>>s)||s=="q"){
            break;
        }
        print(std::cout,tq.query(s))<<std::endl;
    }
}

int main(){
    //输入待查询的.txt文件
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