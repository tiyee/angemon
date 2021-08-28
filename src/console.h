//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_CONSOLE_H
#define ANGEMON_CONSOLE_H
#include "headers.h"
#include "socket_utils.h"
#include "global.h"
extern angemon::ThreadObject<angemon::EventLoop,10> gThreadObject;

namespace angemon{

    int run();
}


#endif //ANGEMON_CONSOLE_H
