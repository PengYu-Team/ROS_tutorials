//ROS头文件
#include <ros/ros.h>
//包含自定义msg产生的头文件
#include <topic_demo/gps.h>
//ROS标准msg头文件
#include <std_msgs/Float32.h>

void gpsCallback(const topic_demo::gps::ConstPtr &msg)
{  
    //计算离原点(0,0)的距离
    std_msgs::Float32 distance;   //定义distance为float32，采用ros中消息定义规则，也可以使用c++原生的  float distance；
    distance.data = sqrt(pow(msg->x,2)+pow(msg->y,2));   //如果用c++，则不需要.data，因为ros中定义的还是一个类

    //float distance = sqrt(pow(msg->x,2)+pow(msg->y,2));
    ROS_INFO("Listener: Distance to origin = %f, state: %s",distance.data,msg->state.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("gps_info", 1, gpsCallback);
  //接受到消息，采用回调函数gpsCallback处理
  //ros::spin()用于调用所有可触发的回调函数。将进入循环，不会返回，类似于在循环里反复调用ros::spinOnce()。
  ros::spin();    //阻塞，相当于死循环，反复查看回调函数队列里有没有东西，如果有就处理清空
  //非阻塞ros::spinOnce()，只查看一次队列
  return 0;
}

