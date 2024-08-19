#include "TextQuery.h"
#include "QueryResult.h"
//逐行读取输入文件，建立单词到行号的映射
TextQuery::TextQuery(std::ifstream &is):file(new std::vector<std::string>)
{
    std::string text;
    while(getline(is,text)){//逐行读取
        file->push_back(text);//保存行文本
        int n=file->size()-1;//保存当前行号
        std::istringstream line(text);//文本分解为单词
        std::string word;
        while(line>>word){
            auto &lines=wm[word];//如果单词不在map中则插入新单词，注意返回mapped_type为默认初始化的智能指针（为空）
            if(!lines){
                lines.reset(new std::set<line_no>);//若插入新单词则分配一个新的set对象
            }
            lines->insert(n);//插入行号
        }
    }
}

TextQuery::~TextQuery(){}
//查询单词
QueryResult TextQuery::query(const std::string &sought)const{
    static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);//局部静态变量，存储查找失败的结果
    auto loc=wm.find(sought);
    if(loc==wm.end()){
        return QueryResult(sought,nodata,file);
    }
    else{
        return QueryResult(sought,loc->second,file);
    }
}