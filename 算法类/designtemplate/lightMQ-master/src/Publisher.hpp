
#ifndef _PUBLISHER_HPP__
#define _PUBLISHER_HPP__

#include "socketx.hpp"
#include "Protocol.hpp"
//��Ϣ�ķ�����
class Publisher
{
    public:
        Publisher(std::shared_ptr<socketx::Connection> conn)
        : conn_(conn), connected(true)//ʹ�ó�ʼ���б��ʼ�����Ӻ�����״̬
        {
            
        }
		//������Ϣ�Ľӿڣ��ṩ�������Ϣ����
        void publish(std::string topic, std::string msg);
        void unpublish(std::string topic);
    
    private: 
        std::vector<std::string> topic_;//��Ϣ��������
        std::shared_ptr<socketx::Connection> conn_;//���ӵĶ���
        bool connected;//�Ƿ�������״̬
};


#endif
