#ifndef POLLER_HPP
#define POLLER_HPP

#include "utilx.hpp"
#include "Event.hpp"


namespace socketx
{
    /*Forward declaration*/
    class EventLoop;

    class Poller
    {
        public:
            Poller();
            ~Poller();

            /*Wrapper function of Linux/Unix poll
            * Return a vector of active events
            */
            std::vector<std::shared_ptr<Event>> poll();//��poll�ķ�װ���᷵��һ�������¼���ָ������

            /*Update or delete an event from eventList*/
            void updateEvent(std::shared_ptr<Event> event);
            void deleteEvent(std::shared_ptr<Event> event);

            void setTimeout(int timeout)
            {
                timeout_ = timeout;
            }

        private:
            //std::vector<Event *> eventsList;
            std::vector<struct pollfd> pollfdList;//���������������fd�ļ�����������
            std::map<int, std::shared_ptr<Event>> eventsMap; //�¼�map

            int timeout_;//��ʱʱ��
    };
}
#endif
