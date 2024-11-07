#ifndef MATHTOOL_H
#define MATHTOOL_H

#include "Combat2024_AllTeam.h"
#include <cmath>

float p2pDistance(cv::Point2f a,cv::Point2f b);             //两点距离
inline  float radian2angle(float radian);                           //弧度制转角度制
inline  float angle2radian(float angle);                            //角度制转弧度制
void float2char(char *a,float b);                                    //float转化为char
void char2float(char *a,float &b);
float mean(float a,float b);

#endif //MATHTOOL_H
