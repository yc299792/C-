#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP


#include "socketx.hpp"
#include <unordered_map>
#include <set>

using socketx::Message;
using socketx::Connection;
using std::cout;
using std::endl;
using std::string;
using std::cin;

/*
 * protocol����Ҫ������Ϣ�����л��ͷ����л���topic��ʾ����
 * ||cmd_size||topic_size||msg_size||cmd||topic||msg||
 * ������:
 * sub, pub, unsub, unpub, broadcast, regist
 */


class Protocol
{
    private:
        size_t cmd_size; 
        size_t topic_size;
        size_t msg_size;
        std::string cmd_;
        std::string topic_;
        std::string msg_;
    public:
        std::string getCmd(){
            return cmd_;
        }
        std::string getTopic(){
            return topic_;
        }
        std::string getMsg(){
            return msg_;
        }

        Protocol()=default;
        Protocol(std::string cmd, std::string topic, std::string msg);

        size_t getBytesLength();
        Message serialization();
        void deserialization(const Message &msg);
};


#endif
