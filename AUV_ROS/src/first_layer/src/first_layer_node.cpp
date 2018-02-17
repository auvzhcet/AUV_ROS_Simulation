#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include "iostream"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/CameraInfo.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "tf/transform_broadcaster.h"
using namespace cv;
using namespace std;

class camera
{
public:
	Mat image;
    void getImageFromCam(Mat dummy_image ){
		image = dummy_image;
	}
	void publishToTopic(sensor_msgs::ImagePtr msg,image_transport::Publisher pub){
		pub.publish(msg);
	}
};
string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}
bool isfront_front(){
    string serialNo = "85A3C860";
    string result = exec("sudo udevadm info --query=all /dev/video0 | grep 'SERIAL_SHORT'");
    if(result.find(serialNo)){
        return true;
    }
    return false;
}
int main(int argc, char** argv)
{
	ros::init(argc,argv,"first_layer_node");
    ros::NodeHandle n;
    image_transport::ImageTransport front_transport(n);
    image_transport::Publisher pub_front = front_transport.advertise("/frontCamera/image_raw", 1);
    image_transport::ImageTransport bottom_transport(n);
    image_transport::Publisher pub_bottom = bottom_transport.advertise("/bottomCamera/image_raw", 1);
    sensor_msgs::ImagePtr msg_front,msg_bottom;
    ros::Publisher frontCamInfoPub = n.advertise<sensor_msgs::CameraInfo>("/frontCamera/camera_info", 1000);
    ros::Publisher bottomCamInfoPub = n.advertise<sensor_msgs::CameraInfo>("/bottomCamera/camera_info", 1000);
    
    sensor_msgs::CameraInfo cameraFrontInfo_msg;
    tf::TransformBroadcaster br;
    tf::Transform transform;
    ros::Rate rate(10.0);
    
    cameraFrontInfo_msg.height = 480;
    cameraFrontInfo_msg.width = 640;
    cameraFrontInfo_msg.distortion_model = "plumb_bob";
    cameraFrontInfo_msg.D = {0.024129, 0.157053, 0.009537, 0.007385, 0.000000};
    cameraFrontInfo_msg.K = {786.683917, 0.000000, 328.762141, 0.000000, 784.097932, 261.697504, 0.000000, 0.000000, 1.000000};
    cameraFrontInfo_msg.R = {1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000};
    cameraFrontInfo_msg.P = {801.427002, 0.000000, 331.585520, 0.000000, 0.000000, 799.029968, 264.361855, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000};
    int camera_front,camera_bottom;
    if (!isfront_front()){
        camera_front = 0;
        camera_bottom = 1;
    }
    else {
        camera_bottom = 0;
        camera_front = 1;
    }
    Mat frame_front,frame_bottom,dst;
    
    VideoCapture cap_front(camera_front);
    cap_front>>frame_front;
    VideoCapture cap_bottom(camera_bottom);
    cap_bottom>>frame_bottom;

    camera front_cam,bottom_cam;

    while(ros::ok()){
        transform.setOrigin( tf::Vector3(0.0, 0.0, 2.0) );
        transform.setRotation( tf::Quaternion(0, 0, 0) );
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", "/frontCamera"));
        

        frontCamInfoPub.publish(cameraFrontInfo_msg);
    	cap_front>>frame_front;
    	cap_bottom>>frame_bottom;

    	front_cam.getImageFromCam(frame_front);
    	bottom_cam.getImageFromCam(frame_bottom);

		msg_front = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame_front).toImageMsg();
    	msg_bottom = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame_bottom).toImageMsg();
		front_cam.publishToTopic(msg_front,pub_front);
    	bottom_cam.publishToTopic(msg_bottom,pub_bottom);
        imshow("fframe",frame_front);
    	imshow("bframe",frame_bottom);
    	if( waitKey(1) == 13 ) break;
    	ros::spinOnce();
    }
	return 0;
}
