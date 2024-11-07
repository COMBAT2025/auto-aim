#include "TimerBase.h"
#include "time.h"

timer::timer()
{
    start=cv::getTickCount();
    numT=0;
}

std::string timer::showTime()
{
    if(numT>4e9)
        clear();
    update();
    finish=cv::getTickCount();
    double totaltime=(double)(finish-start)/cv::getTickFrequency();
    double fps=(numT/totaltime);
//    std::cout<<numT<<"  "<<totaltime<<" "<<fps<<std::endl;
    char data[20];
    //sprintf(data,"%d fps",int(fps));
    sprintf(data,"%d fps",int(fps));
    std::string result=data;
    //std::cout<<totaltime/numT<<std::endl;
    return result;
}

void timer::showOnImage(cv::Mat image)
{
    cv::putText(image,showTime(),cv::Point(100,100),
                cv::FONT_HERSHEY_SIMPLEX,2.0,cv::Scalar(0,0,255),2,8);
}

void timer::update()
{
    numT++;
}

void timer::clear()
{
    numT=0;
    start=clock_t();
}

void timer::functiontime()
{
    double finish=cv::getTickCount();
    double ft=finish-start;
    std::cout<<"这个函数耗时"<<ft<<"ms"<<std::endl;
}