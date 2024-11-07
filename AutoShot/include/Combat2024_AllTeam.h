#ifndef Combat2024_AllTeam_H
#define Combat2024_AllTeam_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>


#define COLOR_R cv::Scalar(0,0,255)  //color red
#define COLOR_G cv::Scalar(0,255,0)  //color green
#define COLOR_B cv::Scalar(255,0,0)  //color blue
#define COLOR_Y cv::Scalar(0,255,255)             //color yellow
#define COLOR_C cv::Scalar(255,255,0)             //color cyan 青色
#define COLOR_P cv::Scalar(255,0,255)             //color purple
#define COLOR_W cv::Scalar(255,255,255)           //color white
#define COLOR_D cv::Scalar(0,0,0)                 //color dark
#define COMBAT_PRINT(x)                   std::cout << "Mode_Debug:" << x << std::endl;
#define COMBAT_PRINT2(s1,s2)              std::cout << "Mode_Debug"<<" "<< s1 << " : " << s2 << std::endl;
#define SMALLARMOUR  0                      //PNP 解算模式 Mode
#define LARGEARMOUR  1
#define BUFFER       2
#define PI           3.1415926
#define HERO         1
#define ENGINEER     2
#define INFANTRY     3
#define DRONE        6
#define SENTRY       7
#define RED          1    
#define BLUE         0
#define BC  channels[0]
#define GC  channels[1]
#define RC  channels[2]
#define Debug(x)    std::cout<<(#x)<<": "<<x<<std::endl;

typedef std::vector<cv::Point2f> Point2f_v;
typedef std::vector<cv::Point3f> Point3f_v;
typedef short sint;

extern cv::Mat videoSave;
extern char colorFlag;                             //1 red  0 blue
extern unsigned char modeFlag;                     //1 高速  2 低速  3高速小陀螺  4低速小脱落
extern unsigned char lastModeFlag;


#endif //Combat2024_AllTeam_H