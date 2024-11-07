#ifndef SENTRYTRAJECTORYSOLVING_H
#define SENTRYTRAJECTORYSOLVING_H
#include<opencv2/opencv.hpp>
#define gra 9.8
#define k_air 0.0333
const double pi = 3.1415926;
struct  turnAngle{
    short pitchAngle=0;
    short yawAngle=0;
};//云台偏转角度

void outputShow(cv::Mat &src,cv::Mat tvec,turnAngle result);
turnAngle solveEquation(float speed,cv::Mat tvec,float pitch,float yaw);
int forSongziwu(int codeNum,float YawAngle,float dis);


#endif //SENTRYTRAJECTORYSOLVING_H
