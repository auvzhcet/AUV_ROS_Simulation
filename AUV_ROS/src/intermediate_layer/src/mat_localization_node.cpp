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
Mat frame;

int mat_pixels(Mat frame_raw){
    Mat frame_hsv,thres;
    cvtColor(frame_raw,frame_hsv,CV_BGR2HSV);
    inRange(frame_hsv,Scalar(0,100,100),Scalar(10,255,255),thres);
    
}

int pid_img;


void imgCallback(const sensor_msgs::ImageConstPtr& msg){
try
    {
        frame = cv_bridge::toCvShare(msg,"bgr8")->image;
        pid_img = mat_pixels(frame);
        waitKey(30);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "gate_detect_node");
    ros::NodeHandle n;
    image_transport::ImageTransport it_front(n);
    image_transport::Subscriber sub_front;
    ros::Publisher gate_pixels = n.advertise<std_msgs::Int16>("interm/mat_pixels", 1000);
    std_msgs::Int16 msg;
 
    while (ros::ok())
    {
        sub_front = it_front.subscribe("camera/front",5,imgCallback);
    }
    return 0;
}
