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

bool compareArea(Rect boundRect1,Rect boundRect2){
    double i = fabs(boundRect1.area());
    double j = fabs(boundRect2.area());
    return (i<j);
}
Mat frame;

int gate_pixels(Mat frame){
    Mat hsv_frame,hsv_frame_red,hsv_frame_green,hsv_frame_red1,hsv_frame_red2;
    cvtColor(frame,hsv_frame,CV_BGR2HSV);
    Mat interm(frame.size(),CV_8UC3,Scalar(0,0,0));

    inRange(hsv_frame_red,Scalar(0,100,100),Scalar(10,255,255),hsv_frame_red1);
    inRange(hsv_frame_red,Scalar(160,100,100),Scalar(180,255,255),hsv_frame_red2);
    hsv_frame_red = hsv_frame_red1 + hsv_frame_red2;

    inRange(hsv_frame_green,Scalar(35,115,115),Scalar(70,255,255),hsv_frame_green);
    hsv_frame = hsv_frame_red + hsv_frame_green;
    vector<vector<Point> > contours;
    findContours(hsv_frame,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point(0,0));
    vector<Rect> boundRect(contours.size());
    for( size_t i = 0; i < contours.size(); i++ )
    {
        boundRect[i] = boundingRect( Mat(contours[i]));
        rectangle(interm,boundingRect(contours[i]).tl(),boundingRect(contours[i]).br(),Scalar(0,255,255),1);
    }
    sort(boundRect.begin(),boundRect.end(),compareArea);

    Point p;
    p.y = hsv_frame.size().height/2;
    p.x = (boundRect[boundRect.size()-1].br().x+boundRect[boundRect.size()-2].tl().x)/2;
    circle(interm,p,5,Scalar(0,255,0));
    return interm.size().width/2 - p.x;
}

int pid_img;

void imgCallback(const sensor_msgs::ImageConstPtr& msg){
    try
    {
        frame = cv_bridge::toCvShare(msg,"bgr8")->image;
        pid_img = gate_pixels(frame);
        waitKey(30);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "gate_detect_node");
    ros::NodeHandle n;
    image_transport::ImageTransport it_front(n);
    image_transport::Subscriber sub_front;
    ros::Publisher gate_pixels = n.advertise<std_msgs::Int16>("interm/gate_pixels", 1000);
    std_msgs::Int16 msg;
    while (ros::ok())
    {
        sub_front = it_front.subscribe("camera/front",5,imgCallback);
        msg.data = pid_img;
        gate_pixels.publish(msg);
        ros::spinOnce();
    }

    return 0;
}
