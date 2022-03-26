#include "ctre/Phoenix.h"
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "ctre/phoenix/platform/Platform.h"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include "DeviceIDs.h"
#include "ctre/phoenix/motorcontrol/SensorCollection.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;

#define LINEAR_ADJ 1
#define ANGULAR_ADJ 1
#define DRIVE_SCALE 1 // 25%

/*******************************************************************************
****     This node subscribes to the motor values set in ExcvLDrvPwr and 	****
****         ExcvRDrvPwr and sets the motors speeds respectively         	****
****     Subscribers:                                                    	****
****          std_msgs/Float32 ExcvLDrvPwr - left motor value            	****
****          std_msgs/Float32 ExcvRDrvPwr - right motor value              ****
****		  geometry_msgs/Twist cmd_vel   - tport left & right motor power****
*******************************************************************************/

class Listener
{
    public:
        void setPump(const std_msgs::Float32 motorSpeed);

        VictorSPX pumpDrive = {DeviceIDs::PumpTalon};
};

void Listener::setPump(const std_msgs::Float32 motorSpeed)
{
	pumpDrive.Set(ControlMode::PercentOutput, motorSpeed.data);
	
	ctre::phoenix::unmanaged::FeedEnable(100);	
}

int main (int argc, char **argv)
{
    ros::init(argc, argv, "PumpControl");
	ros::NodeHandle n;
	ros::Rate loop_rate(100);

   	phoenix::platform::can::SetCANInterface("can0");

    Listener listener;

   	ros::Subscriber motor_toggle_sub = n.subscribe("PumpToggle", 100, &Listener::setPump, &listener);

   

    while (ros::ok())
	{
        
		ros::spinOnce();
		loop_rate.sleep();
    }
}