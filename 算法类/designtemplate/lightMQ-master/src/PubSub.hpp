#ifndef _PUBSUB_HPP__
#define _PUBSUB_HPP__

#include "socketx.hpp"
#include "Protocol.hpp"

//��������м����
class PubSub{

    public:
        PubSub()=default;

        void subscriberStub(std::shared_ptr<Connection> conn, const Message &msg);//�������߶�����Ϣ�Ľӿ�
        void publisherStub(std::shared_ptr<Connection> conn, const Message &msg);//�������߷�����Ϣ�Ľӿ�
        void filter(std::shared_ptr<Connection> conn, const Message &msg);//��Ϣ�Ĺ�����
        
    private:
        void regist(std::shared_ptr<Connection> conn);//����
        void subscribe(std::shared_ptr<Connection> conn, const std::string &topic);//����
        void unsubscribe(std::shared_ptr<Connection>, const std::string &topic);//ȡ������

        void publish(const std::string &topic, const Message &msg);
        void unpublish(std::shared_ptr<Connection> conn, const std::string &topic);

        void broadcast(const string &topic);

        void checker();
        void publishMsg(const std::string &topic);

        /*
         * key : topicֵ
         * value : ��Ϣ���У�һ��topic��Ӧһ��������msg queue��
         *         msg queueȷ�������е���Ϣ�������еĶ����ߣ����ҽ�����һ��
         */
        std::unordered_map<std::string, std::shared_ptr<socketx::MessageQueue>> queueMap_;

        //�����洢���ӵ�map
        std::unordered_map<std::string, std::vector<std::shared_ptr<Connection>>> channelMap_;


        /*
         * key : ���ӵĿͻ���
         * value : һ�����ϣ����ж��ĵ�����
         */
        std::unordered_map<std::shared_ptr<Connection>, std::set<std::string>> subscriberMap_;
        socketx::ThreadPool pool_;
};
#endif
