#include  "ros/ros.h"
#include "std_msgs/String.h"
#include <Eigen/Eigen>
#include <sstream>
#include "geometry_msgs/PoseStamped.h"
#include "math_utils.h"

using namespace std;
//string topic[num];

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
    ROS_INFO("seq:%d ", msg->header.seq);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "listen_vrpn");
    ros::NodeHandle nh("~");
    string object_name;
    nh.param<string>("object_name", object_name, "UAV");
   
    ros::Subscriber sub = nh.subscribe("/vrpn_client_node/"+ object_name + "/pose", 100, mocap_cb);
    //接受到消息，采用回调函数gpsCallback处理
    //ros::spin()用于调用所有可触发的回调函数。将进入循环，不会返回，类似于在循环里反复调用ros::spinOnce()。
    ros::spin();    //阻塞，相当于死循环，反复查看回调函数队列里有没有东西，如果有就处理清空
    //非阻塞ros::spinOnce()，只查看一次队列
    return 0;
}