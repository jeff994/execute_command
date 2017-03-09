#include "ros/ros.h"
#include "std_msgs/String.h"
#include "serial/serial.h"
#include <sstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <iterator>


std::vector<int> commands_list; 
int command_buffer = 100;
int receiving_index = 0;
int executing_index = 0;
serial::Serial my_serial;

void initSerial()
{
	my_serial.setPort("/dev/serial/by-id/usb-Arduino__www.arduino.cc__Arduino_Uno_85438363039351206271-if00");
 	serial::Timeout::simpleTimeout(1000);
 	serial::Timeout timeout	= serial::Timeout::simpleTimeout(1000);
 	my_serial.setTimeout(timeout);
 	my_serial.setBaudrate (9600);
}

bool openSerial()
{
	if(my_serial.isOpen())
		return true; 
	try 
	{
		my_serial.open();
	}
	catch (serial::SerialException ex)
 	{
 		ROS_INFO("open fail : ex.what()");
 	}
	
	return my_serial.isOpen();
} 


void callback(const sensor_msgs::JointState::ConstPtr& data)
{
	commands_list[receiving_index] = data.data;
    ROS_INFO("I heard %s", commands_list[receiving_index]);
	receiving_index = (receiving_index + 1) % command_buffer;
	if (open_serial())
	{
           	status_pub.publish("driver 1");
        	ROS_INFO("command :" + data.data + " written to port");
	        ser.write(data.data);
	}

}


void init_command_buffer(int size)
{
	int count = commands_list.size();
	if (count == size)
		return;

	for (int i = 0; i < (size-count); i++)
	{
		commands_list.push_back("");
	}

}


void executer()
{
	rospy.init_node("commad_executer", anonymous=True);
    rospy.Subscriber("command", String, callback);
    rospy.spin();
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "driver_obstacle");
  	ros::NodeHandle n;
  	ros::Publisher status_pub = n.advertise<std_msgs::String>("status", 100);
	init_command_buffer(command_buffer);
	executer();
}