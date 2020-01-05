#include "PubSub.hpp"
#include <glog/logging.h>

void PubSub::filter(std::shared_ptr<Connection> conn, const Message &msg)
{
    Protocol pro;
    pro.deserialization(msg);
    std::string cmd = pro.getCmd();
    LOG(INFO) << "Received a message of cmd: " << cmd << " topic: " << pro.getTopic() << "msg:" << pro.getMsg();
    
    if(cmd=="sub" || cmd=="unsub" || cmd=="regist")
        subscriberStub(conn, msg);
    else
        publisherStub(conn, msg);
}

void PubSub::subscriberStub(std::shared_ptr<Connection> conn, const Message &msg){
    Protocol pro;
    pro.deserialization(msg);
    if(pro.getCmd()=="sub")
        subscribe(conn,pro.getTopic());
    else if(pro.getCmd()=="unsub"){
        unsubscribe(conn,pro.getTopic());
    }else if(pro.getCmd()=="regist")
        regist(conn);
    else
    {
        LOG(ERROR) << "subscriberStub: no such command " << pro.getCmd() <<" ...";
    }
}

/*
*1���ȴ���һ��Э����󣬽���msg
*2��Ȼ����ȥ�ж����������������Ӧ�Ĳ���
*
*/
void PubSub::publisherStub(std::shared_ptr<Connection> conn, const Message &msg)
{
    Protocol pro;
    pro.deserialization(msg);
    if(pro.getCmd()=="pub")
        publish(pro.getTopic(), msg);
    else if(pro.getCmd()=="unpub")
        unpublish(conn,pro.getTopic());
    else
    {
        LOG(ERROR) << "publisherStub: no such command...";
    }
}

//�������û�����ӻᱻ�ܾ�
void PubSub::regist(std::shared_ptr<Connection> conn)
{
    if(!subscriberMap_.count(conn))
    {
        subscriberMap_[conn] = std::set<std::string>();
        LOG(INFO) << "A subscriber registed " << conn->getFD() << "...";
    }
}

void PubSub::subscribe(std::shared_ptr<Connection> conn, const std::string &topic)
{
    if(subscriberMap_.count(conn))
    {
        subscriberMap_[conn].insert(topic);//key��conn��value��һ������
    }
    else
    {
        subscriberMap_[conn] = std::set<std::string>();//��һ�ζ�����Ҫ�ȼ���conn��map��
        subscriberMap_[conn].insert(topic);
    }
    //�����д洢�����������Ϣ��{key��value}�ķ�ʽ�����һ����ҪΪ���ⴴ��һ����Ϣ����
    if(!queueMap_.count(topic))
    {
        //queueMap_[topic]->addConnection(conn);
        queueMap_.insert({topic,std::make_shared<socketx::MessageQueue>()});//����һ������ָ��ά��MessageQueue����
    }

    queueMap_[topic]->addConnection(conn);//����topic��ȥ���һ��conn
    
    LOG(INFO) << "Subscriber " << conn->getFD() << "subscribes topic " << topic.c_str();

}

void PubSub::unsubscribe(std::shared_ptr<Connection> conn, const std::string &topic)
{
    if(subscriberMap_.count(conn))
    {
        subscriberMap_[conn].erase(topic);
    }
    else
    {
        return;
    }
    
    if(queueMap_.count(topic))
    {
        queueMap_[topic]->removeConnection(conn);
    }
    else
    {
        LOG(INFO) << "Unsubscribe error: no such a topic....";
    }
}

void PubSub::publish(const std::string &topic, const Message &msg)
{
   LOG(INFO) << "called publish with content " << msg.getData();

   /*
    *  ���û���κ��˶��ģ��Ǿ͹㲥�������Ѿ�ע��Ŀͻ���
    */
#if 1
   if(!queueMap_.count(topic))
   {
        broadcast(topic);
        queueMap_.insert({topic,std::make_shared<socketx::MessageQueue>()});
    }
#endif
    queueMap_[topic]->recv(msg);
    if(pool_.getIdleThreadNum()==0) pool_.addThread();
    pool_.submit(std::bind(&PubSub::publishMsg, this, topic));
}

void PubSub::unpublish(std::shared_ptr<Connection> conn, const std::string &topic)
{
    if(channelMap_.count(topic))
    {
        auto it = std::find(channelMap_[topic].begin(),channelMap_[topic].end(),conn);
        if(it == channelMap_[topic].end())
        {
            LOG(INFO) << "unpublish: no such a connection in this topic...";
        }
        else
        {
            channelMap_[topic].erase(it);
        }
    }
    else
    {
        LOG(INFO) << "Unpublish error: no such a topic....\n";
    }
}

void PubSub::broadcast(const string &topic)
{
    Protocol pro("broadcast", topic, "");
    Message msg = pro.serialization();
    for(auto it=subscriberMap_.begin();it!=subscriberMap_.end();++it)
    {
        if((it->second).count(topic)) continue;
        else
        {
            it->first->sendmsg(msg);
        }
    }
}

void PubSub::checker()
{

}

void PubSub::publishMsg(const std::string &topic)
{
    LOG(INFO) << "enter publishMsg";
    while(!queueMap_[topic]->empty())
    {
        if (1 != queueMap_[topic]->send())
        {
            //LOG(ERROR) << "send msg failed.";
        }
    }
}
