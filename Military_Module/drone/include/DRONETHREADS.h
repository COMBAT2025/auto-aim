#ifndef DRONETHREADS_H
#define DRONETHREADS_H

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


//DRONE_thread 1 串口接收
void droneReadSerialport(SerialPort &tempSerial)
{
    int i=1;
    while(i)
    {
        tempSerial.readBuffer(tempSerial.rxpackage);
    }
}



//DRONE_thread 2 装甲识别、解算、预测、发送
int droneImageDeal(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial)
{
    ArmourFinder armourFinder(tempSetter.armourPara,tempSetter.armourPara.whoIam);
    cv::Mat originFrame;
    int cmode;
    int i=1;
    while(i) {
        tempCamera >> originFrame;
        originFrame *= 20;
        videoSave=originFrame;
        if(tempSetter.armourPara.isShowOriginImage=TRUE)
        {
            cv::imshow("origin", originFrame);
            cv::waitKey(10);
        }
        std::vector<Armour> armours;
        armours=armourFinder.findArmour(tempSetter,originFrame,cmode);
        TargetDecision targetDecision(armours);
//        cv::Mat_<double> tvec(3,1);
//        PNPSolver pnpSolver;
//        tvec=pnpSolver.armourDistance(targetArmour.concors,targetArmour.name[1],tempSetter.cameraDistortion);
//        PTZCameraAngleOffset ptzResultAngle=trajectionResolving(tempSerial.rxpackage.pitch,
//                                                                tempSerial.rxpackage.yaw,tempSerial.rxpackage.speed,tvec);
//        tempSerial.txpackage.yaw=ptzResultAngle.yawAngle;
//        tempSerial.txpackage.pitch=ptzResultAngle.pitchAngle;
//        if(tempSerial.rxpackage.autoshot=1)
//            tempSerial.writeBuffer(tempSerial.txpackage);
    }
    return 0;
}



//DRONE_thread 3 视频录制、日志撰写
int droneVideoDataSave()
{
    cv::VideoWriter writer(getTimePictureC(),cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25.0, cv::Size(800, 600));
    while(1)
    {
        writer << videoSave;
    }
}


#endif //DRONETHREADS_H
