#include "ros/ros.h"
#include "lazik/my_msg.h"

#include <sstream>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "Status_generator"); 
  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<lazik::my_msg>("rover/status", 100);

  ros::Rate loop_rate(0.5);

  uint8_t status = 0;

  while (ros::ok())
  {
    lazik::my_msg msg;

    msg.LED_status.data = status;
    ROS_INFO("%u", msg.LED_status.data);
    status=++status%3;

    pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    
  }


  return 0;
}
