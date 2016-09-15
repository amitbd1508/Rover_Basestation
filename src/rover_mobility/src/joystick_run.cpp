#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float64MultiArray.h>

#define MAX_LIN_SPEED 230
#define MAX_ANGLE 30

ros::Publisher mob_pub;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {

	std::vector<float> inp = joy -> axes;
	std::vector<int> but = joy -> buttons;
	
	std::vector<double> out(2, 0);
	out[0] = MAX_LIN_SPEED * inp[1];
	out[1] = - MAX_ANGLE * inp[3]; 
	
	if(but[0] == 1) {
		out[1] = 0;
	}

	std_msgs::Float64MultiArray outMsg;
	outMsg.data = out;
	mob_pub.publish(outMsg);
}

int main(int argc, char** argv) {

	ros::init(argc, argv, "joystick_driver");
	ros::NodeHandle _nh;
	
	mob_pub = _nh.advertise<std_msgs::Float64MultiArray>("/rover/mobility_directives", 100);
	ros::Subscriber joy_sub = _nh.subscribe("/joy", 100, joyCallback);
	ros::Rate loop_rate(100);

	ros::spin();

	return 0;
}
