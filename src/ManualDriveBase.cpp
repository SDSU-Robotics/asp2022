#include "ctre/Phoenix.h"
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/Twist.h"
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

		Listener();

        void getLSpeed(const std_msgs::Float32 lspeed);
        void getRSpeed(const std_msgs::Float32 rspeed);
		
		void setMotorOutput();
		void getMotorStatus(const std_msgs::Float32 motor);
		bool motor_status = 0;
		float leftPower = 0;
		float rightPower = 0;
		int targetPos = 1000;
		
        TalonSRX leftDrive = {DeviceIDs::ExcvDrvLTal};
        TalonSRX rightDrive = {DeviceIDs::ExcvDrvRTal};

};

int main (int argc, char **argv)
{
    ros::init(argc, argv, "ManualDriveBase");
	ros::NodeHandle n;
	ros::Rate loop_rate(100);

    // sets the message to the message variable
	std_msgs::Float32 l_current_msg;
	std_msgs::Float32 r_current_msg;

	phoenix::platform::can::SetCANInterface("can0");
	
	Listener listener;

	ros::Subscriber l_drive_sub = n.subscribe("LDrvPwr", 100, &Listener::getLSpeed, &listener);
	// Left speed of excavator drive power

	ros::Subscriber r_drive_sub = n.subscribe("RDrvPwr", 100, &Listener::getRSpeed, &listener);
	// Right speed of excavator drive power

	ros::Publisher wheelPos = n.advertise<std_msgs::Int32>("WheelPos", 100);

	std_msgs::Int32 pos;

	while (ros::ok())
	{
		listener.setMotorOutput();
		
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}

Listener::Listener()
{
	rightDrive.SetInverted(true);
}

void Listener::getLSpeed(const std_msgs::Float32 lspeed)
{
	leftPower = lspeed.data;
}

void Listener::getRSpeed(const std_msgs::Float32 rspeed)
{
	rightPower = rspeed.data;
}

void Listener::setMotorOutput()
{
	leftDrive.Set(ControlMode::PercentOutput, leftPower);
	rightDrive.Set(ControlMode::PercentOutput, rightPower);
	
	ctre::phoenix::unmanaged::FeedEnable(100);	
}

void Listener::getMotorStatus(const std_msgs::Float32 motor)
{
	motor_status = motor.data;
}