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
    bool findWho;
    void newBin(Setter &tempSetter,cv::Mat &inPut);
    float avg_X;
    float angle_z;
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
