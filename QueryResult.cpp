#include "QueryResult.h"

QueryResult::~QueryResult(){}
//输出查询结果
std::ostream &print(std::ostream& os,const QueryResult &qr){
    auto plural=qr.lines->size()>1?"times":"time";
    os<<qr.sought<<" occuers "<<qr.lines->size()<<" "<<plural<<std::endl;
    for(auto num: *qr.lines){
        os<<"\t(line"<<num+1<<")"<<*(qr.file->begin()+num)<<std::endl;
        
    }
    return os;
}