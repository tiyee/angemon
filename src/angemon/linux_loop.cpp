#include "linux_loop.h"
#include <sys/epoll.h>
using angemon::event::E_READ;
using angemon::event::E_WRITE;
namespace angemon {
    namespace linux {
        Loop::Loop() {
            _efd = epoll_create(MAXEVENTS);
            if (-1 == crRet) {
                LOG_ERR("epoll create fail")
            }


        }
        void Loop::register_(Event *&evt) {
            epoll_event * ev = new epoll_event;

            ev.events = evt->ev_flags==E_READ?EPOLLIN:EPOLLOUT;
            ev.data.fd=evt->fd;
            if(epoll_ctl(_efd, EPOLL_CTL_ADD, evt->fd, &ev) != 0){
               LOG_ERR("epoll_ctl fail fd=%d",evt->fd)
                return;
            }
            _active[evt->fd] = evt;

        }
        void Loop::unregister_(Event *&evt) {
            delete evt->data;
            evt->data = nullptr;
            epoll_ctl(_efd,  EPOLL_CTL_DEL,evt->fd, nullptr);
            int fd_ = evt->fd;
            auto iter = _active.find(fd_);
            _active.erase(iter);
        }
        void Loop::modify(Event *&evt, int ev) {
            auto ev = (epoll_event *)evt->data;
            ev.events = evt->ev_flags==E_READ?EPOLLIN:EPOLLOUT;
            epoll_ctl(_efd,  EPOLL_CTL_MOD,evt->fd, ev);
            evt->ev_flags = ev;
        }
        vector<Event*> Loop::poll() {
            struct epoll_event events_in[MAXEVENTS];
            int n =epoll_wait(_efd, events_in, MAXEVENTS,-1);
            std::vector<Event *> items;
            for (int i = 0; i < n; ++i) {
               if( events_in[i].events & EPOLLIN){
                   item = _active[fd];
                   item->ev_flags = E_READ;
               }
                if( events_in[i].events & EPOLLOUT){
                    item = _active[fd];
                    item->ev_flags = E_WRITE;
                }

                items.push_back(item);
            }
            return items;
        }

    }
}

