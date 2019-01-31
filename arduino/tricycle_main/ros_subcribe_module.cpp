#include "ros_subscribe_module.h"
#include "ros.h"
#include "geometry_msgs/Pose2D.h"

ros::NodeHandle  nh1;

geometry_msgs::Pose2D msg;
TSPose pose;
ros::Subscriber<geometry_msgs::Pose2D> nxt_pose("Pose_next", &messageCb );

void messageCb ()
{
  pose.x = msg.x;
  pose.y = msg.y;
  pose.phi = pose.phi
}

char hello[13] = "hello world!";

void ros_subscribe_setup()
{
  nh1.initNode();
  nh1.subscribe(nxt_pose);
}

void ros_subscribe_module()
{
    nh1.spinOnce();
}