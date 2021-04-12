//masterhome
//ROS头文件
#include <ros/ros.h>
#include<std_msgs/String.h>
#include<sstream>
#include <boost/format.hpp>
#include "geometry_msgs/PoseStamped.h"
#include "math_utils.h"

using namespace std;
//string topic[num];
int node_num,uav_num;

void topicCallback(const  geometry_msgs::PoseStamped::ConstPtr &msg)
{  
}

int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv, "masternode");

   ros::NodeHandle nh("~");

  nh.param<int>("node_num", node_num, 1) ;   
  nh.param<int>("uav_num", uav_num, 1) ;   

  ros::Subscriber sub[node_num*uav_num+1];

int count = 1;
for(int j = 1; j <=uav_num; j++)
{
   for(int i = 1; i <=node_num; i++) 
 {    
    boost::format fmt1("/uav%d/node%d/pub/topic1");
    sub[count] = nh.subscribe((fmt1%(j)%(i)).str(), 1, topicCallback);
    count ++;
}
}

  ros::spin();
  return 0;
} 

