#ifndef ROS_SUBSCRIBE_MODULE
#define ROS_SUBSCRIBE_MODULE

typedef struct
{
    float x;
    float y;
    float phi;
}TSPose;

extern TSPose pose;

void ros_subscribe_setup();
void ros_subscribe();

#endif