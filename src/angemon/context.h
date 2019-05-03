#ifndef CONTEXT_H
#define CONTEXT_H
#include"common.h"
using std::string;
namespace angemon {
struct Context
{
    string buff ;
    Context(string buff_){
        this->buff = std::move(buff_);
    }
};

}
#endif // CONTEXT_H
