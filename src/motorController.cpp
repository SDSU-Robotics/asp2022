//general includes needed for ROS files
#include <string>
#include <unistd.h>

#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "std_msgs/UInt16.h"
#include <sensor_msgs/Joy.h>
#include "std_msgs/Bool.h"
//include to access JoyMap.h file wich stores all the button mapping for Joystick
#include "JoyMap.h"
#include "DeviceIDs.h"
//required
using namespace std;

#define DRIVE_SCALE 1

//class to store functions and data in. Required
class Listener
{
//decalre all functions
public:
	void joyListener(const sensor_msgs::Joy::ConstPtr& Joy);
	void getJoyVals(bool buttons[], double axes[]) const;
	void F32Toggle(const bool b, bool &current, bool &on, std_msgs::Float32 &msg);
	
	
	

//declare array (1)
private:
    bool _buttons[12] = { 0 };
	double _axes[6] = { 0 };
	
};

//function to fill array (1) with Joystick INput
void Listener::joyListener(const sensor_msgs::Joy::ConstPtr& Joy)
{
	for (int i = 0 ; i < 12; i++)
		_buttons[i] = Joy->buttons[i];

    for (int i = 0; i < 6; i++)
        _axes[i] = Joy->axes[i];
}

//function to fill array (2) with values from array(1)
void Listener::getJoyVals(bool buttons[], double axes[]) const
{
    for (int i = 0; i < 12; i++)
        buttons[i] = _buttons[i];

    for (int i = 0; i < 6; i++)
        axes[i] = _axes[i];
}

void Listener::F32Toggle(const bool b, bool &current, bool &on, std_msgs::Float32 &msg)
{
	bool prev = current;
	current = b;

	if (prev && !current)
	{
		on = !on;
		ROS_INFO("PRESSED");
	}
		
	if (on)
	{
		ROS_INFO("ON");
		msg.data = 1;
	}
	else
	{
		ROS_INFO("OFF");
		msg.data = 0;
	}
}







//function needed by all .cpp files
int main (int argc, char **argv)
{
	//required for ROS to work. The name inside the quotations is the name of this node. Cannot be the same as other node names
    ros::init(argc, argv, "ManualDriveController");
	ros::NodeHandle n;
	ros::Rate loop_rate(100);

	//initialize the Listner class to access the function we need. Required
	Listener listener;

	//listen to any joystick input and call the appropriate listener function accordingly. Required
	ros::Subscriber joySub = n.subscribe("joy", 100, &Listener::joyListener, &listener);
	
	//create array (2). Required
	bool buttons[12];
	double axes[6];

	

	int pumpAct = {JoyMap::PumpActToggle};
	bool pumpActCurrent = false; // initialized to false
	bool pumpActOn = false; // initialized to false

	

	//get the number corresponding to the joystick's left stick and right stick
	int Lstick_Yaxis = {JoyMap::LeftDrive};
    int Rstick_Yaxis = {JoyMap::RightDrive};

	

	// publishers: one for each message that we are broadcasting. The name within the quations must be unique (this is the TOPIC).
    ros::Publisher r_drive_pub = n.advertise<std_msgs::Float32>("RDrvPwr", 100);
	ros::Publisher l_drive_pub = n.advertise<std_msgs::Float32>("LDrvPwr", 100);
	ros::Publisher pump_toggle_pub = n.advertise<std_msgs::Float32>("PumpToggle", 100);
	

	// messages: one for each publisher above. Data type must match that of the publisher
    std_msgs::Float32 l_speed_msg;
    std_msgs::Float32 r_speed_msg;
	std_msgs::Float32 pump_toggle_msg;
	

	//required for ROS to work
	while (ros::ok())
	{
        // fill array (2) with values from array (1)
		listener.getJoyVals(buttons, axes);

		// get controller values
		float speed = axes[1]; // left Y
		float turn = 1 * axes[3]; // right X

		// get speed data
		l_speed_msg.data = axes[Lstick_Yaxis] * DRIVE_SCALE; // left up/down stick
		r_speed_msg.data = axes[Rstick_Yaxis] * DRIVE_SCALE; // right up/down stick
		// publish speed data
		l_drive_pub.publish(l_speed_msg);
		r_drive_pub.publish(r_speed_msg);

	

		// toggle A button to toggle linear actuator
		listener.F32Toggle(buttons[pumpAct], pumpActCurrent, pumpActOn, pump_toggle_msg);
		pump_toggle_pub.publish(pump_toggle_msg);

		
		

		

		//required for ROS to work
		ros::spinOnce();
		loop_rate.sleep();
	}
	//required for .cpp to work
	return 0;
}