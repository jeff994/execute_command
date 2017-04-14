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
	my_serial.setPort("/dev/serial/by-id/usb-Arduino__www.arduino.cc__Arduino_Uno_75630313536351217041-if00");
 	serial::Timeout::simpleTimeout(1000);
 	serial::Timeout timeout	= serial::Timeout::simpleTimeout(1000);
 	my_serial.setTimeout(timeout);
 	my_serial.setBaudrate (115200);
}

bool openSerial()
{
	if(my_serial.isOpen())
		return true; 
	my_serial.open();
	return my_serial.isOpen();
}


void commandCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
  if (openSerial())
  {
  	my_serial.write(msg->data);	
  }
}


/*void executer()
{
	rospy.init_node("commad_executer", anonymous=True);
    rospy.Subscriber("command", String, callback);
    rospy.spin();
}*/


int main(int argc, char **argv)
{
	ros::init(argc, argv, "commad_executer");
  	ros::NodeHandle n;
	initSerial();
  	ros::Subscriber sub = n.subscribe("command", 1000, commandCallback);
  	ros::spin();
  	return 0; 
}
