#ifndef ANGLESOLVER_H
#define ANGLESOLVER_H

#include "MathTool.h"
#include<opencv2/opencv.hpp>

struct panTiltZoomCameraOffsetAngle{
    float pitchAngle;
    float yawAngle;
};

float angleBinarySearch(float minAngle,float maxAngle);

panTiltZoomCameraOffsetAngle trajectoryResolving(cv::Mat transitionVector,float pitchangle,float yawangle,float initVolecity);

#endif //ANGLESOLVER_H
