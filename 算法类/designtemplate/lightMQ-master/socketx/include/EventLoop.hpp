#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include "utilx.hpp"

#include "Event.hpp"
#include "Poller.hpp"


namespace socketx
{
    class EventLoop//ѭ����
    {
        public:
            EventLoop();
            ~EventLoop();

            /* Main Loop
            *  The Loop will stop when stop flag is set to true
            */
            void loop();
            void quit() { stop = true; }

            /*Update events by invoke poller's function*/
            void updateEvent(std::shared_ptr<Event> event);
            void deleteEvent(std::shared_ptr<Event> event);

        private:
            std::vector<std::shared_ptr<Event>> activeEvents;
            std::shared_ptr<socketx::Poller> poller;//����ָ�����poller
            std::atomic<bool> stop;//�����������һ�������Ķ�����Ϊ��ѭ��
    };
}
#endif