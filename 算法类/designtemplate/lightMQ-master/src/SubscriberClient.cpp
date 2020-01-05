
#include "Subscriber.hpp"
//�����ߵĿͻ���
class SubscriberClient
{
    public:
        SubscriberClient(socketx::EventLoop *loop, std::string hostname, std::string port)
        : loop_(loop), 
          hostname_(hostname),
          port_(port),
          client_(std::make_shared<socketx::Client>(loop, hostname, port))
        {
			//bind�����ķ���ֵ��һ������ָ��Ҳ����һ���ɵ��õĶ��󣬲�����һ������ָ�룬�������Ĳ����б���һ������Ϊthisָ�룬�ڶ�������������ָ��.
            client_->setHandleConnectionFunc(std::bind(&SubscriberClient::handleConnection, this, std::placeholders::_1));//���ô���Ļص�����
            client_->setHandleCloseEvents(std::bind(&SubscriberClient::handleCloseEvents, this, std::placeholders::_1));

            /*Get file descriptor of stdin and regist it into EventLoop*/
            cout<<"Input <Cmd> <Channel> : " << endl;
            int fd = fileno(stdin);
            stdinConn = std::make_shared<socketx::Connection>(loop_, fd);
            stdinConn->setHandleReadEvents(std::bind(&SubscriberClient::stdinReadEvents, this, std::placeholders::_1));//��ȡ�¼��Ĵ�����
            stdinConn->registReadEvents();
        }

        void start()
        {
            client_->start();//�ͻ�������
        }

        void handleConnection(std::shared_ptr<socketx::Connection> conn)
        {
            printf("New connection comes, we are going to set read events!!!\n");
            client_->setHandleReadEvents(std::bind(&SubscriberClient::handleReadEvents, this, std::placeholders::_1));
            subscriber_ = std::make_shared<Subscriber>(conn);
            subscriber_->regist();
        }

        void handleReadEvents(std::shared_ptr<socketx::Connection> conn)
        {
            socketx::Message msg = conn->recvmsg();
            printf("recved msg : %s", msg.getData());
            if(msg.getSize()==0)
            {
                conn->handleClose();
                return;
            }

            subscriber_->filter(msg);
        }
        
        void handleCloseEvents(std::shared_ptr<socketx::Connection> conn)
        {
            printf("Close connection...\n");
            loop_->quit();
        }

        void stdinReadEvents(std::shared_ptr<socketx::Connection> conn)
        {
            std::string cmd, topic;
            if( cin >> cmd >> topic)
            {
                if(cmd=="sub") subscriber_->subscribe(topic);
                else if(cmd=="unsub") subscriber_->unsubscribe(topic);
            }
            else
                printf("Read error from stdin....\n");
        }

    private:
        std::shared_ptr<Subscriber> subscriber_;
        std::shared_ptr<socketx::Connection> stdinConn;//���ӵĶ���
        socketx::EventLoop *loop_;//���������¼��Ķ���
        std::shared_ptr<socketx::Client> client_;//�ͻ��˶���������·������������������
        std::string hostname_;//����ip�˿ں�
        std::string port_;
};

//�����ߵĿͻ���
int main(int argc, char **argv)
{
    if(argc!=3)
    {
        fprintf(stderr,"usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    std::string hostname(argv[1]);//����
    std::string port(argv[2]);//�˿ں�
    socketx::EventLoop loop;//����һ��Eventloop����
    SubscriberClient client(&loop, hostname, port);
    client.start();//��ȥ���ӷ����
    loop.loop();//ѭ���������ӵĸ����¼���д���ӵ��¼�

    return 0;
}