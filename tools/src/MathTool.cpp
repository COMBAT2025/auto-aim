#include "../include/MathTool.h"


  float  p2pDistance(cv::Point2f a,cv::Point2f b){
   return sqrtf(((a.x-b.x)*(a.x-b.x))+((a.y-b.y)*(a.y-b.y)));
}

inline  float radian2angle(float radian){
    float angle=180*radian/PI;
    return angle;
}

inline  float angle2radian(float angle)
{
    float radian=(PI*(angle/180));
    return radian;
}

union fandc{
    float f;
    char c[4];
};//float和char的联合体
void float2char(char *a,float b)
{
    fandc tr;
    tr.f=b;
    *a=tr.c[0];
    *(a+1)=tr.c[1];
    *(a+2)=tr.c[2];
    *(a+3)=tr.c[3];
}//float转化为char
void char2float(char *a,float &b)
{
    fandc tr;
    tr.c[0]=*a;
    tr.c[1]=*(a+1);
    tr.c[2]=*(a+2);
    tr.c[3]=*(a+3);
    b=tr.f;
}//char转化为float

float mean(float a,float b)
{
    return (a+b)/2;
}


