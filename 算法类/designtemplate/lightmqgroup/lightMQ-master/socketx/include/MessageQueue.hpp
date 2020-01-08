#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

#include "socketx.hpp"

namespace socketx
{
    class MessageQueue
    {
        public:
            MessageQueue()=default;

            ssize_t send();
            void recv(Message msg);

            void addConnection(std::shared_ptr<Connection> conn);
            void removeConnection(std::shared_ptr<Connection> conn);
            bool empty() const{ return queue_.empty();}
			int getGroupSize(){return sendGroup.size();}

        private:
            squeue<Message> queue_;//��Ϣ����
            //std::vector<std::shared_ptr<Connection>> sendList;//ά����һ����Ϣ���͵Ľ������б�,��������
            std::vector<std::vector<std::shared_ptr<Connection>>> sendGroup;//ά���Ŷ�����
    };
}

#endif