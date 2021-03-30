//node1发送topic1～20
//ROS头文件
#include <ros/ros.h>
#include<std_msgs/String.h>
#include<sstream>

using namespace std;
//string topic[num];

void topicCallback(const std_msgs::String::ConstPtr &msg)
{  
    ROS_INFO("node1: I heard : [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv, "node1");

  //实例化句柄，初始化node
  ros::NodeHandle nh;

  //定义发布的频率 
  ros::Rate loop_rate(1.0);

  //发布topic数量
  int num = 20;
  ros::Publisher pub[num+1];
  std_msgs::String msg[num+1];

  //接收msg
  int subnum = 60;
  ros::Subscriber sub[subnum+1];

 for(int i = 1; i <=num; i++) 
 {
    //创建publisher    
    //std_msgs::String为需要public的类型
    //topic1   topic的名称，1为缓存队列
    std::string temp = "topic_1_" ;
   string s = to_string(i);
   temp+=s;
    pub[i] = nh.advertise<std_msgs::String>(temp, 1);   
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
    sub[i] = nh.subscribe(temp, 1, topicCallback);
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
    sub[i] = nh.subscribe(temp, 1, topicCallback);
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
    sub[i] = nh.subscribe(temp, 1, topicCallback);
 }

  //循环发布msg
  //ros::ok表示ros没有关就一直执行循环
  while (ros::ok())
  {
    for(int i = 1; i <=num; i++) 
    {
        std::string temp = "node1 : topic_1_" ;
        string s = to_string(i);
        temp+=s;
        msg[i].data = temp;
        ROS_INFO("%s ",  msg[i].data.c_str());     //相当于printf，显示数据
        //以1Hz的频率发布msg
        pub[i].publish(msg[i]);
    }
    ros::spinOnce();
    //根据前面定义的频率, sleep 1s
    loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  }

  return 0;
} 

