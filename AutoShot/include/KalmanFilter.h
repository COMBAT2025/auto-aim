#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "Combat2024_AllTeam.h"
#include "Setter.h"

class Predict:public cv::KalmanFilter
{
public:
    double deltaTime;
    int    stateNum;
    int    measureNum;
public:
    Predict(PredictModel model,int key);
    cv::Mat predict2(double deltaTime);
    float smooth(float curValue);
    cv::Mat & autoPredict(cv::Mat& measurement);
    void myInit(PredictModel model);
};

#endif //KALMANFILTER_H