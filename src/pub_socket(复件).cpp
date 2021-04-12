//头文件
#include <boost/format.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include  "ros/ros.h"
#include "std_msgs/String.h"
#include "ROS_tutorials/DroneState.h"
#include <sstream>
#include "geometry_msgs/PoseStamped.h"
#include "math_utils.h"
#include <pthread.h>  //多线程

// #include "swarm_control_utils.h"
// #include "message_utils.h"

using namespace std;
//参数声明
int swarm_num;
const int max_swarm_num =10; // indicate max num
string uav_name[max_swarm_num+1];
int uav_id[max_swarm_num+1];
ROS_tutorials::DroneState State_uav[max_swarm_num+1];
ros::Subscriber drone_state_sub[max_swarm_num+1];
char const *servInetAddr = "192.168.1.3"; //sever ip
string data;
char sendline[1024];
int socketfd,connfd;
struct sockaddr_in sockaddr;
const int MAX_LINE = 128;
pthread_t recv_tid , send_tid;


//函数声明
void drone_state_cb1(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[1] = *msg; }
void drone_state_cb2(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[2] = *msg; }
void drone_state_cb3(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[3] = *msg; }
void drone_state_cb4(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[4] = *msg; }
void drone_state_cb5(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[5] = *msg; }
void drone_state_cb6(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[6] = *msg; }
void drone_state_cb7(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[7] = *msg; }
void drone_state_cb8(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[8] = *msg; }
void drone_state_cb9(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[9] = *msg; }
void drone_state_cb10(const ROS_tutorials::DroneState::ConstPtr& msg) { State_uav[10] = *msg; }
void (*drone_state_cb[max_swarm_num+1])(const ROS_tutorials::DroneState::ConstPtr&)={NULL,drone_state_cb1,drone_state_cb2,
    drone_state_cb3,drone_state_cb4,drone_state_cb5,drone_state_cb6,drone_state_cb7,drone_state_cb8,drone_state_cb9,drone_state_cb10};

void *recv_message(void *fd)
{
	int sockfd = *(int *)fd;
	while(1)
	{
		char buf[MAX_LINE];
		memset(buf , 0 , MAX_LINE);
		int n,cnum;
		if((n = recv(sockfd , buf , MAX_LINE , 0)) == -1)
		{
			// perror("recv error.\n");
            close(sockfd);
			continue;
		}//if
		buf[n] = '\0';		
		//若收到的是exit字符，则代表退出通信
		// if(strcmp(buf , "byebye.") == 0)
		// {
		// 	printf("Ground Station closed.\n");
		// 	close(sockfd);
		// 	continue;
		// }//if
        cnum = strlen(buf) ;
        if(cnum>10) 
        {
            close(sockfd);
			continue;
		}
		printf("\nGround Station: %s\n", buf);
        close(sockfd);
	}//while
}


//主函数
int main(int argc, char **argv)
{
    ros::init(argc, argv, "swarm_ground_station");
    ros::NodeHandle nh("~");

    nh.param<int>("swarm_num", swarm_num, 1);
    for(int i = 1; i <= swarm_num; i++) 
    {
        // 设置无人机名字，none代表无
        boost::format fmt1("uav%d_name");
        nh.param<string>((fmt1%(i)).str(), uav_name[i], "/none");
        boost::format fmt2("uav%d_id");
        nh.param<int>((fmt2%(i)).str(), uav_id[i], 0);
        {
            /* code for True */
        }
        
        // 订阅
        //cout << uav_name[i] + "/prometheus/swarm_command" << endl;           //什么意思嗷？
        // drone_state_sub[i] = nh.subscribe<ROS_tutorials::DroneState>(uav_name[i] + "/prometheus/drone_state", 10, drone_state_cb[i]);
        drone_state_sub[i] = nh.subscribe<ROS_tutorials::DroneState>("/" + uav_name[i] +  "/DState", 10, drone_state_cb[i]);
    }
    
        //send by socket
    socketfd = socket(AF_INET,SOCK_STREAM,0);
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(10004);
    inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);

    if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 ) {
        printf("connect error %s errno: %d\n",strerror(errno),errno);
        printf("client connect failed!\n");
    }

    int  sha;
    sha = pthread_create(&recv_tid , NULL , recv_message, &socketfd) ;
    // cout<<sha<<endl;

    // if(pthread_create(&recv_tid , NULL , recv_message, &socketfd) == -1)
    // {
    //         perror("pthread create error.\n");
    //         continue;
    // }

    boost::format fmt3("uav%d,%f,%f,%f,%f,%f,%f,%f,%f,%f");

    
    while(ros::ok())
    {
        ros::spinOnce();
        // cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>> Formation Flight Station <<<<<<<<<<<<<<<<<<<<<<<<< "<< endl;
        for(int i = 1; i <= swarm_num; i++)
        {
            // printf("send message to server: ");
            data = (fmt3%(i)%(State_uav[i].position[0])%(State_uav[i].position[1])%State_uav[i].position[2]%
                (State_uav[i].velocity[0])%(State_uav[i].velocity[1])%(State_uav[i].velocity[2])%
                (State_uav[i].attitude[0])%(State_uav[i].attitude[1])%(State_uav[i].attitude[2])).str();
            // cout << data << endl;
            strcpy(sendline,data.c_str());

            if((send(socketfd,sendline,strlen(sendline),0)) < 0)
            {
                // printf("send mes error: %s errno : %d\n",strerror(errno),errno);
                // printf("client send failed!\n");
            }
            close(socketfd);
        }
        sleep(2.0); // frequence
    }
    return 0;
}