#ifndef EVENT_H
#define EVENT_H
#include"context.h"
#include<functional>
using angemon::Context;
namespace angemon {
namespace event {
enum STATE{
    E_READ  =		0x1,
   E_WRITE 	=	0x2,
    E_SIGNAL		=0x4,
    E_TIMEOUT		=0x8,
 E_EDGE 		=	0x10,/* edge triggered */
 E_ONCE		=	0x20,/* one-time event */
 E_IN_REACTOR 	=0x40,/* indicates whether the event is in the reactor */
/*
* The timer event's initial timerheap_idx value,
* indicates that the event is not in the heap.
*/
    E_OUT_OF_TIMERHEAP =0,
};
struct Event{
    const int fd;
    short ev_flags;
    Context* ctx;
    void * data;
    std::function<int(Context*)> cb ;
    Event(int fd_,short flag_):fd(fd_),ev_flags(flag_),ctx(nullptr),data(nullptr){};


};
}

}
#endif // EVENT_H
