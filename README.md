**主机**

- 节点1
  - 订阅100个话题
    - /uav1/node1/pub/topic1
    - /uav1/node2/pub/topic1
    - ...
    - /uav1/node5/pub/topic1
    - /uav2/node1/pub/topic1
    - /uav2/node2/pub/topic1
    - ...
    - /uav2/node5/pub/topic1
    - ...
    - 订阅所有从机的节点1-5发布的第一个话题，共40个话题



**从机1**

- 节点1：
  - 发布20个话题
    - /uav1/node1/pub/topic1
    - ...
    - /uav1/node1/pub/topic20
  - 接收20个话题
    - /uav1/node2/pub/topic1
    - ...
    - /uav1/node2/pub/topic20

- 节点2：
  - 发布20个话题
    - /uav1/node2/pub/topic1
    - ...
    - /uav1/node2/pub/topic20
  - 接收20个话题
    - /uav1/node3/pub/topic1
    - ...
    - /uav1/node3/pub/topic20

- 节点3：
  - 发布20个话题
    - /uav1/node3/pub/topic1
    - ...
    - /uav1/node3/pub/topic20
  - 接收20个话题
    - /uav1/node4/pub/topic1
    - ...
    - /uav1/node4/pub/topic20

- 节点4：
  - 发布20个话题
    - /uav1/node4/pub/topic1
    - ...
    - /uav1/node4/pub/topic20
  - 接收20个话题
    - /uav1/node5/pub/topic1
    - ...
    - /uav1/node5/pub/topic20
- 节点5：
  - 发布20个话题
    - /uav1/node5/pub/topic1
    - ...
    - /uav1/node5/pub/topic20
  - 接收20个话题
    - /uav1/node1/pub/topic1
    - ...
    - /uav1/node1/pub/topic20

**从机2**

- 节点1：
  - 发布20个话题
    - /uav2/node1/pub/topic1
    - ...
    - /uav2/node1/pub/topic20
  - 接收20个话题
    - /uav2/node2/pub/topic1
    - ...
    - /uav2/node2/pub/topic20
- 节点2
- 节点3
- 节点4
- 节点5





**任务说明**：

- 所有话题格式使用[geometry_msgs](http://docs.ros.org/en/api/geometry_msgs/html/index-msg.html)/PoseStamped，请百度该格式，发布前要赋值；订阅得到后也要存储到本地并打印
- 所有从机节点运行频率为20Hz
- 地面主机节点运行频率为1Hz，需打印出来所有接收到的话题
- 基于上述规则一共创建8个从机，话题前缀分别为/uav1 to /uav8
- 进阶要求：只允许编写一个cpp文件，请善用参数赋值及for循环语句
- 进阶要求：可以学习使用下cout 和 Eigen库
- 话题前缀可以通过launch中设定group来指定，可参考/Kongdijiqun/Simulator/gazebo_simulator/launch_cxy/sitl_cxy_case1_8uav.launch
- 额外增加一个节点，只订阅/vrpn消息，即/vrpn_client_node/uav1/pose ~/vrpn_client_node/uav8/pose 



**测试说明：**

- 可先在自己本地电脑测试
- ROS多机（多台电脑）测试