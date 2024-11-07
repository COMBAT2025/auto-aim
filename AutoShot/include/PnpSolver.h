#ifndef PNPSOLVER_H
#define PNPSOLVER_H

#include "Combat2024_AllTeam.h"
#include "Setter.h"

class PNPSolver {

private:
          //空间坐标的标识符
    //static const cv::Point3f spacePixel[3][4];
    Point3f_v spacePixel[3];    //表示空间中的坐标（通过装甲板的长宽计算得到）
    // cv::Point2f flatPixel[4];

public:
    PNPSolver();  //无参构造函数
    PNPSolver(const Setter::ArmourSize as[3]);  //传入装甲板的尺寸的构造函数
    void clear();                               //清空
    //Point2f_v points 都是图像上的坐标/
    cv::Mat armourDistance(Point2f_v points,char Mode,CameraDistortion cal,cv::Mat &rvec);//解算大装甲和小装甲的距离mode 0、1 小、大
    cv::Mat bigBufferDistance(Point2f_v points,CameraDistortion cal);   //解算大符装甲板的距离

};


#endif //PNPSOLVER_H
