/*
 * Node jednoczesnie Subscriber i Publisher. 
 *
 * Realizuje konwersje wiadomosci wejsciowej (binardna), na wiadomosc interfejsu CAN.
 * Subscriber:
 *    Odbiera wiadomosc typu uint8 z topica „/rover/status”.
 *        0 -> Sterowanie manualne
 *        1 -> Autonomia
 *        2 -> Osiągnięto cel
 * Publisher:
 *    Korzysta z bilioteki "ros_canopen". 
 *    Ustawia wartosci ramki "can_msgs/Frame" oraz dodaje status diody LED do wiadomosci.
 *    Wiadomosc jest wysylana na topic "sent_messages", ktory jest zdefiniowany przez biblioteke "ros_canopen".
 *    Standartowym parametrem "sent_messages" jest "can0", lecz da sie go zmienic na porty o innej numeracji.
*/

#include "ros/ros.h"
#include "lazik/my_msg.h"
#include "can_msgs/Frame.h"

#include <socketcan_bridge/topic_to_socketcan.h>
#include <socketcan_bridge/socketcan_to_topic.h>
#include <socketcan_interface/socketcan.h>

void Callback(const lazik::my_msg::ConstPtr& msg){

  ROS_INFO("I heard: [%u]", msg->LED_status.data);    //Zwraca odebrana wiadomosc do konsoli

  /*Deklaracja Publishera na topic "sent_messages"*/
  ros::NodeHandle roscan;
  ros::Publisher pub = roscan.advertise<can_msgs::Frame>("sent_messages", 100);
  
  can_msgs::Frame can_frame;

  can_frame.id=0x0F;                  //CAN ID - ustawia na 15
  can_frame.is_extended = true;
  can_frame.is_rtr = false;
  can_frame.is_error = false;
  can_frame.dlc = 1;                  //Ile zostanie wyslanych ramek danych (jedna ramka)
  can_frame.data[0]=msg->LED_status.data;   //Ustawia pierwsza ramke na status lazika/diody
                                            //Wiadomosc ta jest odczytywana jako dwucyfowa liczba szestastkowa (zakres 00-FF)(binarnie od 0-255 tak samo jak uint8)
                                            
  /*W tej petli ustawia ramke CAN i wysyla ja. Program dziala rowniez bez petli while, odbiera wiadomosc i jednorazowo ja wysyla.
    Gdy petla istnieje to nadaje ciagle ta SAMMA wiadomosc, w przypadku gdy nic nowego nie odbiera z topicu "rover/status"*/
   while(ros::ok()){                    

    pub.publish(can_frame);             //Wysyla wiadomosc
    ROS_INFO("Has sent messages to can0 ID: %d ", can_frame.id);    //Potwierdzenie gdzie wiadomosc zostala wyslana

    ros::Duration(2).sleep();           //Odstep pomiedzy wiadomosciami wynosi 2 sekundy
    ros::spinOnce();
  }
}

int main(int argc, char **argv){

  ros::init(argc, argv, "Converter");

  /*Deklaracja Subscribera topicu "rover/status"*/
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("rover/status", 100, Callback);
  ros::spin();

  return 0;
}