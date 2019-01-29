#include "opencv2/video/tracking.hpp"
#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <iostream>


static const std::string OPENCV_WINDOW = "Raw Image window";
static const std::string OPENCV_WINDOW_1 = "Edge Detection";
static cv::Mat prevgray;
using namespace cv;
using namespace std;

class floor_detection
{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;
    

    public:
    
    floor_detection(): it_(nh_)
    {
        image_sub_ = it_.subscribe("/usb_cam/image_rect_color",1,&floor_detection::imageCb,this);
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

    void detect_floor(cv::Mat frame)
    {
        using namespace cv;
        using namespace std;
        Mat dst, gray, flow, cflow;
        //namedWindow("flow", 1);
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if( prevgray.data )
        {
        calcOpticalFlowFarneback(prevgray, gray, flow, 0.4, 1, 12, 2, 8, 1.2, 0);
        cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
        drawOptFlowMap(flow, cflow, 5, 1.5, Scalar(0, 255, 0));
        //imshow("flow", cflow);
        }
        //if(waitKey(30)>=0)
        //break;
        std::swap(prevgray, gray);
        dst = cv::Scalar::all(0);
        frame.copyTo(dst,cflow);
        dst.copyTo(frame);
        //src_hsv.copyTo(frame);

    	cv::imshow(OPENCV_WINDOW, frame);
    	cv::imshow(OPENCV_WINDOW_1, dst);
    	cv::waitKey(3);

    }

    static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
    {
        for(int y = 0; y < cflowmap.rows; y += step)
            for(int x = 0; x < cflowmap.cols; x += step)
            {
                const Point2f& fxy = flow.at<Point2f>(y, x)*10;
                line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                        color);
                circle(cflowmap, Point(x,y), 1, Scalar(0,0,0), -1);
            }
    }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "floor_detector");
  floor_detection ic;
  ros::spin();
  return 0;
}


/*
#include "opencv2/video/tracking.hpp"
#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


static void help()
{
    cout <<
            "\nThis program demonstrates dense optical flow algorithm by Gunnar Farneback\n"
            "Mainly the function: calcOpticalFlowFarneback()\n"
            "Call:\n"
            "./fback\n"
            "This reads from video camera 0\n" << endl;
}
static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
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
    //image_pub_.publish(cv_ptr->toImageMsg());
    }
}

static void detect_floor(cv::Mat frame)
{
    help();
    Mat prevgray, gray, flow, cflow
    namedWindow("flow", 1);
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    if( prevgray.data )
    {
    calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
    cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
    drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
    imshow("flow", cflow);
    }
    if(waitKey(30)>=0)
    break;
    std::swap(prevgray, gray);
}

int main(int, char**)
{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_(nh_);
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;


    image_sub_ = it_.subscribe("/usb_cam/image_raw",1,imageCb);
    ros::spin();
    return 0;
}
*/
