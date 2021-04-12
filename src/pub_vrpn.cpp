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
#include <pthread.h>

using namespace std;
//string topic[num];
int num,listen_num;
string uav_name,node_id,listen_node;

void topicCallback(const  geometry_msgs::PoseStamped::ConstPtr &msg)
{  
}

int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv, "talker");

  //实例化句柄，初始化node
   ros::NodeHandle nh("~");

  nh.param<string>("uav_name", uav_name, "/none");
  nh.param<string>("node_id", node_id,"node0");
  nh.param<int>("num", num, 1) ;     //每个节点发布topic数量
  nh.param<string>("listen_node", listen_node,"node1");
  nh.param<int>("listen_num", listen_num, 1) ;     //每个节点发布topic数量
  ros::Publisher pub[num+1];
  ros::Subscriber sub[listen_num+1];
  geometry_msgs::PoseStamped Start[num+1];

for(int i = 1; i <=listen_num; i++) 
        {   
          boost::format fmt1("/topic%d");
          sub[i] = nh.subscribe("/"+uav_name+"/"+listen_node+"/pub"+(fmt1%(i)).str(), 1, topicCallback);
        }

for(int i = 1; i <=num; i++) 
        {   
  //自定义gps msg
  Start[i].header.seq = 0;
  Start[i].pose.position.x = 1.0;
  Start[i].pose.position.y = 1.0;
  Start[i].pose.position.z = 0.0;
  Start[i].pose.orientation.x = 0.0;
  Start[i].pose.orientation.y = 0.0;
  Start[i].pose.orientation.w = 1.0;
  boost::format fmt2("/topic%d");
  pub[i] = nh.advertise<geometry_msgs::PoseStamped>( "/"+uav_name+"/"+node_id+"/pub"+(fmt2%(i)).str(), 1);
        }
  //定义发布的频率 
  ros::Rate loop_rate(20.0);
  //循环发布msg
  //ros::ok表示ros没有关就一直执行循环
  while (ros::ok())
  {
    //以指数增长，每隔1秒更新一次
    ROS_INFO("Talker: OK ");     //相当于printf，显示数据
    //以1Hz的频率发布msg
    for(int i = 1; i <=num; i++) {
    pub[i].publish(Start[i]);
    }
    //根据前面定义的频率, sleep 1s
    loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  }

  return 0;
} 
