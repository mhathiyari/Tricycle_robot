#include "ros_subscribe_module.h"
#include "ros.h"
#include "geometry_msgs/Pose2D.h"

ros::NodeHandle  nh1;

std_msgs::String str_msg;
ros::Publisher next_pose("chatter", &str_msg);

char hello[13] = "hello world!";

void ros_subscribe_setup()
{
  nh1.initNode();
  nh1.advertise(chatter);
}

void ros_subscribe_module()
{
    geometry_msgs::Pose2D msg;

    str_msg.data = hello;
    chatter.publish( &str_msg );
    nh1.spinOnce();
}