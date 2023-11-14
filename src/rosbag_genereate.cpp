#include "rosbag_generate.h"

namespace rosbag_generate{

    BagFileProcessor::BagFileProcessor(std::string dir){
        file_dir = dir;
        getBagFileList();
    }

    std::vector<cv::Mat> BagFileProcessor::getImgMatList(){
        for(auto image_file : files_list){
            cv::Mat image_mat = cv::imread(image_file, cv::IMREAD_COLOR);
            image_list.push_back(image_mat);
        }
        return image_list;
    }

    bool BagFileProcessor::getBagFileList(){
        // traverse all files in file_dir
        DIR* dir = opendir(file_dir.c_str());
        dirent* p = NULL;//定义遍历指针
        while((p = readdir(dir)) != NULL)//开始逐个遍历
        {
            if(strcmp(p->d_name,".")==0 || strcmp(p->d_name,"..")==0)
                continue;
            else
            {
                std::string name = file_dir + std::string(p->d_name);
                // std::cout << name << std::endl;
                files_list.push_back(name);
            }
        }
        // sort files respect to timestamp
        sort(files_list.begin(), files_list.end());
        closedir(dir);
        return true;
    }


}



void run(ros::NodeHandle& nh){
    std::string image_input_path, rosbag_topic_name;
    int publish_rate;
    // rosparam setting
    nh.param("image_input_path", image_input_path, std::string("image_data/illuminance"));
    nh.param("publish_rate", publish_rate, 10);
    nh.param("rosbag_topic_name", rosbag_topic_name, std::string("/image_raw"));

    ros::Publisher image_publisher = nh.advertise<sensor_msgs::Image>(rosbag_topic_name, 1);
    auto rosbag_generater_ptr = 
        std::make_shared<rosbag_generate::BagFileProcessor>(image_input_path);
    const std::vector<cv::Mat>& image_list = rosbag_generater_ptr->getImgMatList();
    int start_frame = 0, image_list_len = image_list.size();
    // msgs
    sensor_msgs::Image image_msgs;
    // rate
    ros::Rate rate(publish_rate);
    std_msgs::Header header;

    while(ros::ok() && start_frame < image_list_len){
        header.seq = 1;
        header.stamp = ros::Time::now();
        cv::Mat curr_image = image_list[start_frame];
        cv_bridge::CvImage image_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::TYPE_8UC3, curr_image);
        image_bridge.toImageMsg(image_msgs);
        image_publisher.publish(image_msgs);
        start_frame++;
        rate.sleep();
    }

}

int main(int argc, char** argv){
    ros::init(argc, argv, "rosbag_generater_node");
    ros::NodeHandle nh("~");
    run(nh);
    return 0;
}