//
// Created by tiyee on 2021/8/25.
//

#include "console.h"


int angemon::run() {

    auto n = gThreadObject.Size();
    LOG("N=%d", n);

    std::thread threads[n + 1]; // first thread is main
    threads[0] = std::thread([]() {
        Socket::StartListen();
    });
    for (uint16_t i = 1; i < n + 1; ++i) {
        threads[i] = std::thread([i]() {
            auto el = gThreadObject.GetObject(i);
            el->SetThreadId(std::this_thread::get_id());
            el->Start();
        });
    }
    for (auto &t: threads) {
        t.join();
    }

    return 0;
}