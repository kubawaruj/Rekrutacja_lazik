#include "ros/ros.h"
#include "lazik/my_msg.h"
#include "can_msgs/Frame.h"

#include <socketcan_bridge/topic_to_socketcan.h>
#include <socketcan_bridge/socketcan_to_topic.h>
#include <socketcan_interface/socketcan.h>

void Callback(const lazik::my_msg::ConstPtr& msg){
  ROS_INFO("I heard: [%u]", msg->LED_status.data);

  ros::NodeHandle roscan;
  ros::Publisher pub = roscan.advertise<can_msgs::Frame>("sent_messages", 100);
  
  while(ros::ok()){
    ROS_INFO("start send can_message");
    can_msgs::Frame can_frame;

    can_frame.id=0x0F;
    can_frame.is_extended = true;
    can_frame.is_rtr = false;
    can_frame.is_error = false;
    can_frame.dlc = 1;
    can_frame.data[0]=msg->LED_status.data;

    pub.publish(can_frame);
    ROS_INFO("has sent messages to %d ", can_frame.id);

    ros::Duration(0.5).sleep();
    ros::spinOnce();
  }
}

int main(int argc, char **argv){

  ros::init(argc, argv, "Converter");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("rover/status", 1000, Callback);
  ros::spin();

  return 0;
}