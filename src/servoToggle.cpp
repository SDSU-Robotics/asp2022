#include "ros/ros.h"
#include "iostream"

#include <sensor_msgs/Joy.h>
#include "JoyMap.h"

#include "std_msgs/UInt16.h"

bool buttons[12] = {0};
double axes[6] = {0};

/*==========================================================*/

 void JoyListener(const sensor_msgs::Joy::ConstPtr& Joy)
 {
   for (int i=0 ; i< 12; i++)
        buttons[i] = Joy->buttons[i];

    for (int i = 0 ; i<6; i++)
        buttons[i] = Joy->axes[i];
 }

/*==========================================================*/
 int Toggle(const bool b, bool &current,bool &on) 
 {
   int toggleData;
   bool prev = current;
   current = b;

   if (prev && !current)
      on = !on;

   // Turns servo to 180 degrees 
   if (on)
      toggleData = 180;
   // Turns servo back to 0 degrees
   else
       toggleData = 0;

    return toggleData;
 }

 /*==========================================================*/

 int main(int argc, char **argv)
 {
     ros::init(argc, argv, "servoControl");
     ros::NodeHandle n;
     ros::Rate loop_rate(100);

     ros::Subscriber joySub = n.subscribe("joy", 100, JoyListener);

     std_msgs::UInt16 servoControlData;

     // Creates the publisher to send the message
     ros::Publisher servoControlPub = n.advertise<std_msgs::UInt16>("servoControl",100);
     
     int servoDegree = {JoyMap::Servo};
     bool buttonCurrent = false;
     bool buttonPress = false;

     while (ros::ok())
     {
         // stores the toggle data into servoControlData
         servoControlData.data = Toggle(buttons[servoDegree], buttonCurrent, buttonPress);
         
         // publishes the toggle data to the arduino code
         servoControlPub.publish(servoControlData);
         ros::spinOnce();
         loop_rate.sleep();
     }
     return 0;
 }
