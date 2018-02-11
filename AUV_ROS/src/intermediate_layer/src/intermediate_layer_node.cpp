#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include "iostream"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "std_msgs/Int16.h"


using namespace cv;
using namespace std;
void gate_pixels_callback(const std_msgs::Int16::ConstPtr& msg){
    int pixels = msg->data;
}
int main(int argc, char *argv[])
{
    ros::init(argc, argv, "master_node");
    ros::NodeHandle nh;
    ros::Subscriber gate_pixels ;

    while (ros::ok())
    {
        gate_pixels = nh.subscribe("/interm/gate_pixels",1000,gate_pixels_callback);
        
        ros::spinOnce();
    }
    
    
    return 0;
}