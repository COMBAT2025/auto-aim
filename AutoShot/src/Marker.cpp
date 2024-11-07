#include "Marker.h"
#include "MathTool.h"

Marker::Marker() {}

Marker::Marker(cv::RotatedRect &rRect)
{
    x=rRect.center.x;
    y=rRect.center.y;
    center=rRect.center;

    cv::Point2f vertex[4];
    rRect.points(vertex);

    if(p2pDistance(vertex[0],vertex[1])<p2pDistance(vertex[1],vertex[2]))
    {
        LedUp=(vertex[0]+vertex[1])/2;
        LedDown=(vertex[2]+vertex[3])/2;
    }
    else
    {
        LedUp=(vertex[0]+vertex[3])/2;
        LedDown=(vertex[1]+vertex[2])/2;
    }
    if(LedDown.y<LedUp.y)
    {
        cv::Point2f tmp;
        tmp=LedUp;
        LedUp=LedDown;
        LedDown=tmp;
    }
    if(rRect.size.height>rRect.size.width)
    {
        angle=(rRect.angle+90);
        size=rRect.size.height;
    }
    else
    {
        angle=(rRect.angle);
        size=rRect.size.width;
    
    }
    area=rRect.size.width*rRect.size.height;
    if(angle<0)
        angle+=180;
}

void Marker::show(cv::Mat &frame)
{
    cv::circle(frame,LedUp,5,COLOR_R,-1);
    cv::circle(frame,LedDown,5,COLOR_G,-1);
    char data[100];
    sprintf(data,"%d %d",int(size),int(angle));
    cv::putText(frame,data,LedUp,cv::FONT_HERSHEY_COMPLEX,0.7,cv::Scalar(0,0,255),2,0);

}


//std::vector<Marker>  findMarker(cv::Mat &mat)
//{
//
//
//
//}