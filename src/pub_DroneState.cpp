#include  "ros/ros.h"
#include "std_msgs/String.h"
#include "ROS_tutorials/DroneState.h"
#include <sstream>
#include "geometry_msgs/PoseStamped.h"
#include "math_utils.h"

using namespace std;
//string topic[num];

int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv, "DStatenode");

  //实例化句柄，初始化node
  ros::NodeHandle nh("~");
  string uav_name;
  nh.param<string>("uav_name", uav_name, "/none");

  //自定义gps msg
  ROS_tutorials::DroneState State_uav;
  State_uav.position[0] = 1.0;
  State_uav.position[1] = 2.0;
  State_uav.position[2] = 3.0;
  State_uav.velocity[0] = 4.0;
  State_uav.velocity[1] = 5.0;
  State_uav.velocity[2] = 6.0;
  State_uav.attitude[0] = 7.0;
  State_uav.attitude[1] = 8.0;
  State_uav.attitude[2] = 9.0;

  //创建publisher    
  //topic_demo::gps为需要public的类型
  //gps_info   topic的名称，1为缓存队列
  ros::Publisher pub = nh.advertise< ROS_tutorials::DroneState>("/" + uav_name +  "/DState", 10);

  //定义发布的频率 
  ros::Rate loop_rate(1.0);
  //循环发布msg
  //ros::ok表示ros没有关就一直执行循环
  while (ros::ok())
  {
    //以指数增长，每隔1秒更新一次
    ROS_INFO("Talker: OK ");     //相当于printf，显示数据
    //以1Hz的频率发布msg
    pub.publish(State_uav);
    //根据前面定义的频率, sleep 1s
    loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  }

  return 0;
} 
