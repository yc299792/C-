
#include "Publisher.hpp"
#include <glog/logging.h>

void Publisher::publish(std::string topic, std::string msg)
{
    Protocol pro("pub", topic, msg);//����һ�����󣬰���Э��
    Message pub_msg = pro.serialization();//�ײ�Ὣ��Ϣ�������
    conn_->sendmsg(pub_msg);//ͨ����������ȥ����������Ϣ���ᵽ��������Ȼ�󷢲���MQ
    topic_.push_back(topic);//��topic�����һ������

    LOG(INFO) << "A message of topic" << topic <<" is publishing!";
}

void Publisher::unpublish(std::string topic)
{
    Protocol pro("unpub", topic, "");
    Message pub_msg = pro.serialization();
    conn_->sendmsg(pub_msg);
    //todo delete topic
    //topic_.move(topic);

    LOG(INFO) << "A topic " << topic << "will be unpublished!";
}


