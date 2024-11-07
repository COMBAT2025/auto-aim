#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "Combat2024_AllTeam.h"
#include <cstdlib>
#include "CameraDefine.h"
#include "CameraApi.h"
#include "CameraStatus.h"
#include <sstream>
#include <list>
#include "Setter.h"
#include "MindVisionCamera.h"


class Calibrator {

public:

    std::vector<std::vector<cv::Point3f>> object_points;

    std::vector<std::vector<cv::Point2f>> image_points;

    std::vector<cv::Mat> rvects, tvects;

    //attention!static function can only use static parameters
    static cv::Mat camera_Matrix;

    static cv::Mat dist_Coeffs;

    int number =0;


    Calibrator(Setter::CalibratorPara &temp,MindVisionCamera &tempCamera);

    static void auto_Start(

            const char* image_folders,MindVisionCamera &tempCamera,cv::Size board_size,cv::Size square_size
    );//自动拍照并且保存的程序

    static void save_calibrate_result(

            std::string output_filename
    );



    static void read_calibrate_result(

            std::string output_filename
    );



    static void saveImagePoints(

            std::string output_filename,

            const std::vector<cv::Point2f> imagePoints = std::vector<cv::Point2f>(),

            const std::vector<cv::Point3f> objectPoints = std::vector<cv::Point3f>()
    );



    static void readImagePoints(

            std::string output_filename,

            std::vector<cv::Point2f>& imagePoints,

            std::vector<cv::Point3f>& objectPoints
    );



    static cv::Mat  remap_correct(

            const cv::Mat& a_image

    );



    //why private ? can it work when they are public ?//yeah!
private:

    int addChessBoardCorners(

            std::vector<cv::String> file_list,

            const cv::Size board_size,

            const cv::Size square_size,

            cv::Size& image_size
    );



    double Calibrate(

            cv::Size image_temp_size
    );



    void addPoints(

            std::vector<cv::Point2f> image_corners,

            std::vector<cv::Point3f> object_corners
    );
};


#endif //CALIBRATOR_H
