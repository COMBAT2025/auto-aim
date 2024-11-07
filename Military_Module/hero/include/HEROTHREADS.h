#ifndef IMAGEDEAL_H
#define IMAGEDEAL_H

#include "SerialPort.h"
#include "MindVisionCamera.h"
#include "Combat2024_AllTeam.h"
#include "Setter.h"
#include "MindVisionCamera.h"
#include "SerialPort.h"
#include "ArmourFinder.h"
#include <time.h>
#include "TimerBase.h"
#include "TargetDecision.h"
#include "PnpSolver.h"
#include "TrajectorySolving.h"


//HERO_thread 1 串口接收
void heroReadSerialport(SerialPort &tempSerial)
{
    int i=1;
    while(i)
    {
    tempSerial.readBuffer(tempSerial.rxpackage);
    }
}



//HERO_thread 2 装甲识别、解算、预测、发送
int heroImageDeal(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial)
{
    ArmourFinder armourFinder(tempSetter.armourPara,tempSetter.armourPara.whoIam);
    cv::Mat originFrame,a;
    int i=1;
    while(i) {
        tempCamera >> originFrame;
        videoSave=originFrame;
        std::vector<Armour> armours;
        armourFinder.newBin(tempSetter,originFrame);
        cv::imshow("gray", originFrame);
        cv::waitKey(1);//这个之上为灯条寻找
     }
    return 0;
}




//HERO_thread 3 视频录制、日志撰写
int heroVideoDataSave()
{
    cv::VideoWriter writer(getTimePictureC(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25.0, cv::Size(800, 600));
    while(1)
    {
        writer << videoSave;
    }


}




#endif //ALLIANCE2020_CV_GROUP_IMAGEDEAL_H
