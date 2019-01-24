#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Raw Image window";
static const std::string OPENCV_WINDOW_1 = "Edge Detection";

class floor_detection
{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;

    public:
    floor_detection(): it_(nh_)
    {
        image_sub_ = it_.subscribe("/usb_cam/image_raw",1,&floor_detection::imageCb,this);
        image_pub_ = it_.advertise("/floor_detector/raw_image",1);
        cv::namedWindow(OPENCV_WINDOW);
    }
    
    ~floor_detection()
    {
        cv::destroyWindow(OPENCV_WINDOW);
    }
    void imageCb(const sensor_msgs::ImageConstPtr& msg)
    {
        cv_bridge::CvImagePtr cv_ptr;

        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception& e)
        {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
        }

        if (cv_ptr->image.rows > 400 && cv_ptr->image.cols > 600){

        detect_floor(cv_ptr->image);
        image_pub_.publish(cv_ptr->toImageMsg());

        }
    }

    void detect_floor(cv::Mat img)
    {
        cv::Mat src,src_hsv;
        cv::Mat dst, detected_floor;
        /*int low_H = 0;
        int low_S = 0;
        int low_V = 11;
        int high_H = 60;
        int high_S = 33.3;
        int high_V = 50;

        img.copyTo(src);
        cv::cvtColor(img,src_hsv,CV_BGR2HSV);
        cv::inRange(src_hsv,cv::Scalar(low_H,low_S,low_V),cv::Scalar(high_H,high_S,high_V),detected_floor);
        */
        int low_H = 60;
        int low_S = 80;
        int low_V = 80;
        int high_H = 100;
        int high_S = 130;
        int high_V = 130;

        img.copyTo(src);
        cv::cvtColor(img,src_hsv,CV_BGR2HSV);
        cv::inRange(src_hsv,cv::Scalar(low_H,low_S,low_V),cv::Scalar(high_H,high_S,high_V),detected_floor);
        
        dst = cv::Scalar::all(0);
        img.copyTo(dst,detected_floor);
        dst.copyTo(img);
        //src_hsv.copyTo(img);

    	cv::imshow(OPENCV_WINDOW, src);
    	cv::imshow(OPENCV_WINDOW_1, dst);
    	cv::waitKey(3);

    }

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "floor_detector");
  floor_detection ic;
  ros::spin();
  return 0;
}