#include "EventLoop.hpp"

namespace socketx
{
    EventLoop::EventLoop():
        poller(std::make_shared<Poller>()),stop(false)
    {
        IgnoreSIGPIPE();//����SIGPIPE�ź�
    }

    EventLoop::~EventLoop()
    {

    }

    /* Main Loop
    *  The Loop will stop when stop flag is set to true
    */
    void EventLoop::loop()
    {
        printf("EventLoop starts......\n");
        while(!stop)
        {
            activeEvents.clear();//���һ������
            activeEvents = poller->poll();//һֱ�������ײ�ʹ��poll��ʽ�����ص��ǻ�Ծ�¼�������
            for(auto it=activeEvents.begin(); it!=activeEvents.end(); ++it)
            {
                (*it)->handleEvent();//���ε����¼��Ĵ������������ÿһ���¼��Ĵ������ص�����
            }
        }
        printf("EventLoop ends......\n");
    }

    /*Update events by invoke poller's function*/
    void EventLoop::updateEvent(std::shared_ptr<Event> event)
    {
        poller->updateEvent(event);
    }

    void EventLoop::deleteEvent(std::shared_ptr<Event> event)
    {
        poller->deleteEvent(event);
    }

}
