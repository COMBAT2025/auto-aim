#include "Armour.h"

Armour::Armour() {}

Armour::Armour(Marker &markLeft, Marker &markRight)
{
    if(markLeft.x>markRight.x)
    {
        Marker tmp=markLeft;
        markLeft=markRight;
        markRight=tmp;
    }
    concors.push_back(markLeft.LedUp);
    concors.push_back(markLeft.LedDown);
    concors.push_back(markRight.LedDown);
    concors.push_back(markRight.LedUp);
    center=(markLeft.LedUp+markLeft.LedDown+markRight.LedUp+markRight.LedDown)*0.25;//-cv::Point2f(40,0);
}

void Armour::getIdImage(cv::Mat &frame,cv::Mat &boundingImage)
{
    cv::Rect rect=cv::boundingRect(concors);
    cv::Point br=rect.br(),tl=rect.tl();
    br.y=rect.br().y+rect.size().height/3;
    tl.y=rect.tl().y-rect.size().height/3;
    if(br.y>(frame.rows-1))br.y=frame.rows-1;
    if(br.x>(frame.cols-1))br.x=frame.cols-1;
    if(br.x<0)br.x=0;
    if(br.y<0)br.y=0;
    if(tl.y>(frame.rows-1))tl.y=frame.rows-1;
    if(tl.x>(frame.cols-1))tl.x=frame.cols-1;
    if(tl.x<0)tl.x=0;
    if(tl.y<0)tl.y=0;
    my_height=tl.y-br.y;
    cv::Rect rec(br,tl);
    boundingImage=frame(rec).clone();
    boundingImage*=10;
}

void Armour::show(cv::Mat &frame)
{
    cv::line(frame,concors[0],concors[1],COLOR_C,2,8);
    cv::line(frame,concors[2],concors[3],COLOR_C,2,8);
    cv::line(frame,concors[1],concors[2],COLOR_C,2,8);
    cv::line(frame,concors[0],concors[3],COLOR_C,2,8);
    cv::putText(frame,name+1,concors[2],cv::FONT_HERSHEY_COMPLEX,2.0,cv::Scalar(0,0,255),2,0);
    cv::line(frame,cv::Point(0,center.y),cv::Point(2000,center.y),cv::Scalar(0,0,255),1,8);
    cv::line(frame,cv::Point(center.x,0),cv::Point(center.x,2000),cv::Scalar(0,0,255),1,8);
    //cv::imshow("final",frame);
}

void Armour::getId(int No)
{
    id=No;
    if(id>=49)
    {
        name[1]='L';
        id-=49;
        name[3]=1;
    } else {
        name[1]='S';
        name[3]=0;
    }
    name[0]='0'+id;
    name[2]='\0';
    name[4]=id;

}
