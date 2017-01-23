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

int main(int argc, char **argv)
{
  	ros::init(argc, argv, "driver_obstacle");
  	ros::NodeHandle n;
 	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("driver_obstacle", 1000);

 	initSerial(); 
 	ROS_INFO("start driver obstacle");

 	while (ros::ok())
 	{
		ROS_INFO("start loop");
		if(openSerial() == false)
			ROS_INFO("Serial port is not connected, trying to open agin...");
			usleep(100000);
			continue; 
		std_msgs::String msg;
    	std::string result; 
		
		try
		{
   	 		size_t n_size = my_serial.readline(result);
   	 	} 
   	 	catch (serial::SerialException ex)
   	 	{
   	 		ROS_INFO("reconnect driver serial port: %s",ex.what());
   	 		continue;
   	 	}
		std::stringstream ss(result);
		
		msg.data = result ;
		ROS_INFO("string: %s", msg.data.c_str());
		chatter_pub.publish(msg);

    	ros::spinOnce();

    		//loop_rate.sleep();
		ROS_INFO("End loop");
  	}
  	return 0;
}
