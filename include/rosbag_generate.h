#pragma once

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/version.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <vector>
#include <string>
#include <thread>
#include <std_msgs/String.h>
#include <filesystem>
#include <sensor_msgs/CompressedImage.h>
#include <dirent.h>
#include <algorithm>


namespace rosbag_generate{

class BagFileProcessor{
private:
    std::vector<std::string> files_list;
    std::string file_dir;
    std::vector<cv::Mat> image_list;
    int publish_rate_;
    std::string rosbag_output_dir_;
    
public:
    BagFileProcessor(std::string file_dir);
    // Get file cv::Mat by file_name
    std::vector<cv::Mat> getImgMatList();
    // Put all file in file_dir into list
    bool getBagFileList();
};

}