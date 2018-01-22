#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include "iostream"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class camera
{
public:
	Mat image,img_LAB;
	void getImageFromCam(Mat dummy_image ){
		image = dummy_image;
	}
	void publishToTopic(sensor_msgs::ImagePtr msg,image_transport::Publisher pub){
		pub.publish(msg);
	}
	Mat enhanceImg(){
		cvtColor(image,img_LAB,CV_BGR2Lab);

		return image;
	}
	
};

int main(int argc, char** argv)
{
	ros::init(argc,argv,"first_layer_node");
    ros::NodeHandle n;
    image_transport::ImageTransport front_transport(n);
    image_transport::Publisher pub_front = front_transport.advertise("camera/front", 1);
    image_transport::ImageTransport bottom_transport(n);
    image_transport::Publisher pub_bottom = bottom_transport.advertise("camera/bottom", 1);
    sensor_msgs::ImagePtr msg_front,msg_bottom;

    Mat frame_front,frame_bottom;
    VideoCapture cap_front(0);
    cap_front>>frame_front;
    VideoCapture cap_bottom(1);
    cap_bottom>>frame_bottom;

    camera front_cam,bottom_cam;

    while(ros::ok()){
    	cap_front>>frame_front;
    	cap_bottom>>frame_bottom;

    	front_cam.getImageFromCam(frame_front);
    	bottom_cam.getImageFromCam(frame_bottom);
		
		frame_front = front_cam.enhanceImg();
		frame_bottom = bottom_cam.enhanceImg();
    	
		msg_front = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame_front).toImageMsg();
    	msg_bottom = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame_bottom).toImageMsg();
		front_cam.publishToTopic(msg_front,pub_front);
    	bottom_cam.publishToTopic(msg_bottom,pub_bottom);

    	if( waitKey(1) == 13 ) break;
    	ros::spinOnce();
    }
	return 0;
}