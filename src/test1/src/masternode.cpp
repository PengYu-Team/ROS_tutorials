//masterhome
//ROS头文件
#include <ros/ros.h>
#include<std_msgs/String.h>
#include<sstream>

using namespace std;
//string topic[num];

void topicCallback(const std_msgs::String::ConstPtr &msg)
{  
    ROS_INFO("masternode: I heard : [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv, "masternode");

  //实例化句柄，初始化node
  ros::NodeHandle nh;

  int num = 20;
  //接收msg
  int subnum = 80;
  ros::Subscriber sub[subnum+1];

//node1
 for(int i = 1; i <=num; i++) 
 {
    //创建Subscriber    
    //std_msgs::String为需要public的类型
    //topic1   topic的名称，1为缓存队列
    std::string temp = "topic_1_" ;
    string s = to_string(i);
    temp+=s; 
    sub[i] = nh.subscribe(temp, 1, topicCallback);
 }

//node2
 for(int i = 1; i <=num; i++) 
 {
    //创建Subscriber    
    //std_msgs::String为需要public的类型
    //topic1   topic的名称，1为缓存队列
    std::string temp = "topic_2_" ;
    string s = to_string(i);
    temp+=s; 
    sub[i+20] = nh.subscribe(temp, 1, topicCallback);
 }

//node3
 for(int i = 1; i <=num; i++) 
 {
    //创建Subscriber    
    //std_msgs::String为需要public的类型
    //topic1   topic的名称，1为缓存队列
    std::string temp = "topic_3_" ;
    string s = to_string(i);
    temp+=s; 
    sub[i+40] = nh.subscribe(temp, 1, topicCallback);
 }

//node4
 for(int i = 1; i <=num; i++) 
 {
    //创建Subscriber    
    //std_msgs::String为需要public的类型
    //topic1   topic的名称，1为缓存队列
    std::string temp = "topic_4_" ;
    string s = to_string(i);
    temp+=s; 
    sub[i+60] = nh.subscribe(temp, 1, topicCallback);
 }

  ros::spin();
  return 0;
} 

