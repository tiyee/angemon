#ifndef UNIX_LOOP_H
#define UNIX_LOOP_H

#include "loop.h"

using angemon::base::Loop;
using angemon::event::Event;
using std::map;
using std::vector;
namespace angemon {
    namespace unix {
        class UnixLoop : public Loop {
        private:
            map<int, Event *> _active;
            int _kfd;

        public:
            UnixLoop();

            ~UnixLoop() = default;

            virtual void register_(Event *&evt) final;

            virtual void unregister_(Event *&evt) final;

            virtual void modify(Event *&evt, int ev) final;

            virtual vector<Event *> poll() final;
        };
    } // namespace unix
} // namespace angemon

#endif // UNIX_LOOP_H
