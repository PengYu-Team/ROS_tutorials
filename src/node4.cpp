#include  "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "geometry_msgs/PoseStamped.h"
#include "math_utils.h"

using namespace std;
//string topic[num];

//回调函数
void topicCallback(const std_msgs::String::ConstPtr &msg)
{  
    //实例化句柄，初始化node
    ros::NodeHandle nh;
    int me;
    nh.param<int>("me", me );    //当前节点, 为0则是master
    ROS_INFO("Node%d [ I heard ( %s ) ]", me, msg->data.c_str());
}


void mocap_cb(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    //---------------------------------------vicon定位相关------------------------------------------
    Eigen::Vector3d pos_drone_mocap; //无人机当前位置 (vicon)
    Eigen::Quaterniond q_mocap;
    Eigen::Vector3d Euler_mocap; //无人机当前姿态 (vicon)
    
    Eigen::Vector3f pos_offset;
    ros::Time last_timestamp;
     
    //位置 -- optitrack系 到 ENU系
    //Frame convention 0: Z-up -- 1: Y-up (See the configuration in the motive software)
    int optitrack_frame = 0;
    if (optitrack_frame == 0)
    {
        // Read the Drone Position from the Vrpn Package [Frame: Vicon]  (Vicon to ENU frame)
        pos_drone_mocap = Eigen::Vector3d(msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);  //无人机当前位置 (vicon)

        pos_drone_mocap[0] = pos_drone_mocap[0] - pos_offset[0];
        pos_drone_mocap[1] = pos_drone_mocap[1] - pos_offset[1];
        pos_drone_mocap[2] = pos_drone_mocap[2] - pos_offset[2];
        // Read the Quaternion from the Vrpn Package [Frame: Vicon[ENU]]
        q_mocap = Eigen::Quaterniond(msg->pose.orientation.w, msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z);
    }
    else
    {
        // Read the Drone Position from the Vrpn Package [Frame: Vicon]  (Vicon to ENU frame)
        pos_drone_mocap = Eigen::Vector3d(-msg->pose.position.x, msg->pose.position.z, msg->pose.position.y);
        // Read the Quaternion from the Vrpn Package [Frame: Vicon[ENU]]
        q_mocap = Eigen::Quaterniond(msg->pose.orientation.w, msg->pose.orientation.x, msg->pose.orientation.z, msg->pose.orientation.y); //Y-up convention, switch the q2 & q3
        pos_drone_mocap[0] = pos_drone_mocap[0] - pos_offset[0];
        pos_drone_mocap[1] = pos_drone_mocap[1] - pos_offset[1];
        pos_drone_mocap[2] = pos_drone_mocap[2] - pos_offset[2];
    }

    // Transform the Quaternion to Euler Angles  将四元数变换为欧拉角
    Euler_mocap = quaternion_to_euler(q_mocap);
    last_timestamp = msg->header.stamp;
}


int main(int argc, char **argv)
{
    //用于解析ROS参数，第三个参数为本节点名
    ros::init(argc, argv,"anynode");
    //实例化句柄，初始化node
    ros::NodeHandle nh("~");

    int me  ;
    int nodenum;
    int num ;
    int frequ ;
    string object_name;
    nh.param<int>("uav", uav, 0 ) ;    //无人机几号
    nh.param<int>("me", me, 0 ) ;    //当前节点, 为0则是master
    nh.param<int>("nodenum", nodenum, 2) ;     //一共有多少个节点
    nh.param<int>("num", num, 10) ;     //每个节点发布topic数量
    nh.param<int>("frequ", frequ, 1) ;     //发布频率
    nh.param<string>("object_name", object_name, "UAV");

    ROS_INFO("Hello? Look at me! What are you afraid of?");

    //定义发布的频率 
    ros::Rate loop_rate(frequ);

    //发布topic数量
    ros::Publisher pub[num+1];
    std_msgs::String msg[num+1];

    std::string myname = "/uav" ;
    string s = to_string(uav);
    myname+=s;
    myname+='_topic';
    string s = to_string(me);
    myname+=s; 

      // 【订阅】optitrack估计位置
          for(int i = 1; i <=num; i++) 
        {   
        //std_msgs::String为需要public的类型
        //topic1   topic的名称，1为缓存队列
        std::string temp = "topic_" ;
        string s1 = to_string(me);
        temp+=s1;
        temp+="_";
        string s2 = to_string(i);
        temp+=s2;
        pub[i] = nh.advertise<std_msgs::String>(temp, 1);   
         }
    }
      else 
      {  
                subnum = nodenum*num; 
       }

//创建Subscriber
ros::Subscriber sub[subnum+1];
int count = 1;
for(int j = 1; j <=nodenum; j++)
{
   for(int i = 1; i <=num; i++) 
 {    
    if(j ==me) break;
    //std_msgs::String为需要public的类型
    //topic1   topic的名称，1为缓存队列
    std::string temp = "topic_" ;
    string s3 = to_string(j);
    temp+=s3;
    temp+="_";
    string s4 = to_string(i);
    temp+=s4;
    sub[count] = nh.subscribe(temp, 1, topicCallback);
    count ++;
 }
}

  //循环发布msg
  //ros::ok表示ros没有关就一直执行循环
  while (ros::ok())
  {
    if(me!=0)
    {
          for(int i = 1; i <=num; i++) 
          {
              std::string temp = myname ;
              temp+=" : topic_";
              string s5 = to_string(me);
              temp+=s5;
              temp+="_";
              string s6 = to_string(i);
              temp+=s6;

              msg[i].data = temp;
              ROS_INFO("%s ",  msg[i].data.c_str());     //相当于printf，显示数据
              //以1Hz的频率发布msg
              pub[i].publish(msg[i]);
          }
    }
    ros::spinOnce();
    //根据前面定义的频率, sleep 1s
    loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  }

  return 0;
} 

