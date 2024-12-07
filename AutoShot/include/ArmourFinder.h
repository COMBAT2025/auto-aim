#ifndef ARMOURFINDER_H
#define ARMOURFINDER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "Setter.h"
#include "Armour.h"
#include "Marker.h"
#include "MathTool.h"
#include "Svm.h"

class ArmourFinder {
public:
    ArmourFinder(){}
    ArmourFinder(Setter::ArmourPara &para,bool whoIam);
    void imageBinary(Setter &tempSetter,cv::Mat &inPut,cv::Mat &out);
    double calculateRotationAngle(const Marker& mark1, const Marker& mark2);
    std::vector<Marker> findMark(Setter &tempSetter,cv::Mat &frame);
    std::vector<Armour> findArmour(Setter &tempSetter,cv::Mat &frame,int &cmode);
    std::vector<cv::Point3f> getArmours(cv::Mat &tvec, float &angle_z);
    //std::vector<cv::Point2f> world2pixel(std::vector<cv::Point3f> objectPoints,cv::Mat rvec,cv::Mat tvec,CameraDistortion cal);
    //std::vector<cv::Point2f> world2pixel(std::vector<cv::Point3f> objectPoints,CameraDistortion cal);
    bool findWho;
    void newBin(Setter &tempSetter,cv::Mat &inPut);
    float avg_X;//所有灯条横坐标均值
    float center_X;//建模车的中心横坐标
    float angle_z;//装甲板左右倾斜角
    float otheravg_Y;//虚假装甲板纵坐标均值
    float otheravg_X;//虚假装甲板横坐标均值
     cv::Point2f fakecenter[2];
private:
    float maxDifference;
    float minDifference;
    float maxDistance;
    float minDistance;
    int minContourSize;
    float supLengthWidth;
    float infLengthWith;
    int valueThresh;
    int thresh4Binary;
    float minAngle;
    bool isShowGrayImage;
    bool isShowFinallImage;
    char whoIam;
    Svm svm;

};

#endif //ARMOURFINDER_H
