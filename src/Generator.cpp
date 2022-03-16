/*
Node Publisher - wysyla co 2 sekundy Topiciem "/rover/status" na przemian 3 stany (0,1,2), 
ktore odpowiadaja (0 - sterowany manualnie, 1 - autonomia, 2 - Osiagnieto cel).

Stworzylem wlasna wiadomosc "my_msg", ktora sklada sie z danej "LED_status" typu uin8. 
Dopiero po skonczeniu pisania programu zrozumialem, ze nie musialem tworzyc wlasnej wiadomosci :D.
Skoro jest napisana to szkoda z niej nie skorzystac.

Node jest zrobiony nadprogramowo, potrzebny byl do testowania glownego Noda
*/

#include "ros/ros.h"
#include "lazik/my_msg.h"

#include <sstream>

int main(int argc, char **argv){

  ros::init(argc, argv, "Status_generator"); 

  /*Deklaracja Publishera na topic "rover/status"*/
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<lazik::my_msg>("rover/status", 100);

  ros::Rate loop_rate(0.5);     //Zmienna do petli 2 sekundowej
  ros::Duration(0.5).sleep();   //Stop 0,5 sekundowy, aby "Converter_node" zdazyl przechwicic pierwsza wiadomosc

  uint8_t status = 0;           //Deklaracja stanu lazika

  while (ros::ok()){
    lazik::my_msg msg;

    msg.LED_status.data = status;     //Podstawia status lazika pod status diody
    ROS_INFO("%u", msg.LED_status.data);
    pub.publish(msg);           //Wysyla wiadomosc

    status=++status%3;          //Modulo 3 pozwala na generowanie stanow - 0, 1, 2

    ros::spinOnce();
    loop_rate.sleep();          //Wiadomosc wysylana co 2 sekundy
    
  }

  return 0;
}
