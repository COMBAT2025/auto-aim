//
// Created by roggie on 19-11-1.
//

#ifndef MARKER_H
#define MARKER_H

#include "Combat2024_AllTeam.h"

class Marker {

public:

    Marker();
    Marker(cv::RotatedRect &rRect);
    cv::RotatedRect bouning;
    cv::Point2f LedUp,LedDown;//最高点和最低点
    float angle;//每个灯条与水平方向的夹角
    float size;//理想状态下为灯条的个数
    float area;
    int x,y;
    cv::Point2f center;
    void show(cv::Mat &frame);




};

   //std::vector<Marker>  findMarker(cv::Mat &mat);


#endif //MARKER_H