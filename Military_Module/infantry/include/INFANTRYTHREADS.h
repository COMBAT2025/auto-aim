#ifndef INFANTRYTHREADS_H
#define INFANTRYTHREADS_H

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
#include "KalmanFilter.h"
#include "gimbal_control.h"


//INFANTRY_thread 1 串口接收
void infantryReadSerialport(SerialPort &tempSerial)
{
    int i=1;
    while(i)
    {
        tempSerial.readBuffer(tempSerial.rxpackage);
    }
}



//INFANTRY_thread 2 装甲识别、解算、预测、发送
int infantryImageDeal(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial)
{
  /*PNPSolver pnpSolver(tempSetter.allsize);
   ArmourFinder armourFinder(tempSetter.armourPara,tempSetter.armourPara.whoIam);
   cv::Mat_<double> tvec(3,1);
   int strikeTarget;
   cv::Mat originFrame;
    cv::Mat_<double> tvec(3,1),rvec(3,1);
   float bulletSpeed;
   Predict predicter(tempSetter.AnglePModelY,1);
   static cv::Mat measurement = cv::Mat::zeros(predicter.measureNum,1,CV_32F);
   double fps=60;
   timer Timer;
   int i=1;
   while(i) {
       colorFlag=1;
       colorFlag=tempSerial.rxpackage.color/5;                    //红蓝方读裁判系统时候，这里ArmourFinder要挪过来
        modeFlag =(tempSerial.rxpackage.color-1)%4+1 ;
       if(modeFlag=1||3) bulletSpeed=25.0;
       else              bulletSpeed=15.0;//子弹速度
       tempCamera >> originFrame;
       videoSave=originFrame;
       if(tempSetter.armourPara.isShowOriginImage==TRUE)
       {
           cv::imshow("origin", originFrame);
           cv::waitKey(1);
       }
       std::vector<Armour> armours;
       armours=armourFinder.findArmour(tempSetter,originFrame);
       TargetDecision targetDecision(armours);
       strikeTarget=targetDecision.PrintTarget();
       if(strikeTarget!=-1)
       {
           tvec = pnpSolver.armourDistance(armours[strikeTarget].concors,0,tempSetter.cameraDistortion,rvec);
           float xCamera=tvec.at<double>(2)/1000.0;                       //m       x=z
           float yCamera=tvec.at<double>(1)/(-1000.0);                    //m       y=-y                                                   //coordinate by camera
           float zCamera=tvec.at<double>(0)/1000.0;                       //m       z=x
           cv::Point3f aimArmour;
           aimArmour.x=xCamera;
           aimArmour.y=yCamera;
           aimArmour.z=zCamera;
           gimbalContrl.Init(-0.02,0.02,0.0,0.0,0.0,20.0,-0.04);
           tempSerial.rxpackage.pitch= tempSerial.rxpackage.pitch*PI/180.0;
           tempSerial.rxpackage.yaw= tempSerial.rxpackage.yaw*PI/180.0;
           gimbalContrl.Transform(aimArmour,tempSerial.rxpackage.pitch,tempSerial.rxpackage.yaw);

           cv::Mat prediction = predicter.predict2(0.02);

           measurement.at<float>(0)=(float)tvec.at<double>(0);
           measurement.at<float>(1)=(float)tvec.at<double>(1);
           measurement.at<float>(2)=(float)tvec.at<double>(2);
           predicter.correct(measurement);
           //std::cout<<"pre:"<<prediction<<std::endl;
           double distance=sqrtf(prediction.at<float>(0)*prediction.at<float>(0)+
                  prediction.at<float>(1)*prediction.at<float>(1)+prediction.at<float>(2)*prediction.at<float>(2));
          // double转string
           char dis[100];
           //std::cout<<measurement<<tvec<<"距离";
           //std::cout<<"distance:"<<distance<<std::endl;

           sprintf(dis,"%f",distance);
           Timer.showOnImage(originFrame);
           cv::putText(originFrame,dis,cv::Point(50,50),cv::FONT_HERSHEY_COMPLEX,1.0,cv::Scalar(255,0,255),2,0);
       //    std::cout<<"tvec"<<tvec<<"pre"<<prediction<<std::endl;
//            tvec.at<double>(0)= prediction.at<float>(0);
//            tvec.at<double>(1)= prediction.at<float>(1);
//            tvec.at<double>(2)= prediction.at<float>(2);
        //  以上三行我注释掉了滤波
           float xCamera=tvec.at<double>(2)/1000.0;                       //m       x=z
           float yCamera=tvec.at<double>(1)/(-1000.0);                    //m       y=-y                                                   //coordinate by camera
           float zCamera=tvec.at<double>(0)/1000.0;
           cv::Mat_<double> trans(3,1);
           trans.at<double> (0)=xCamera;
           trans.at<double> (1)=yCamera;
           trans.at<double> (2)=zCamera;

       //COMBAT_PRINT(tempSerial.rxpackage.pitch);
       //COMBAT_PRINT(tempSerial.rxpackage.yaw);
          panTiltZoomCameraOffsetAngle result=trajectoryResolving(trans,tempSerial.rxpackage.pitch,tempSerial.rxpackage.yaw,25);
         //std::cout<<"pitchOffsetAngle:"<<result.pitchAngle<<std::endl<<"yawOffsetAngle:"<<result.yawAngle<<std::endl;
            tempSerial.tx_package.pitch=result.pitchAngle;
            tempSerial.tx_package.yaw=result.yawAngle;
           if(1)
           {
              // tempSerial.txpackage.pitch=0;
                tempSerial.tx_package.pitch=result.pitchAngle;
                tempSerial.tx_package.yaw=result.yawAngle;
               //tempSerial.txpackage.flag=4;
             //  COMBAT_PRINT(tempSerial.txpackage.pitch);
             //  COMBAT_PRINT(tempSerial.txpackage.yaw);
             if(abs(tempSerial.tx_package.pitch)<2)   tempSerial.tx_package.pitch=0;
            if(abs(tempSerial.tx_package.yaw)<2)   tempSerial.tx_package.yaw=0;
             if(tempSerial.tx_package.yaw<-15)      tempSerial.tx_package.yaw=0;
             tempSerial.writeBuffer(tempSerial.tx_package);
               while(tempSerial.tx_package.pitch<1&&tempSerial.tx_package.yaw<1)
               {
                   tempSerial.tx_package.pitch=0;
                   tempSerial.tx_package.yaw=0;
                   tempSerial.tx_package.shot=1;
                   tempSerial.writeBuffer(tempSerial.tx_package);
               }
            }
       }


       if(tempSetter.armourPara.isShowFinallyImage==TRUE)
       {
           cv::imshow("final",originFrame);
           cv::waitKey(1);
       }




   }*/
    return 0;
}

//INFANTRY_thread 3 视频录制、日志撰写
int infantryVideoDataSave()
{
    cv::VideoWriter writer(getTimePictureC(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 90.0, cv::Size(800, 600));
    while(1)
    {
        writer << videoSave;
    }
}


void infantryWriteSerialport(SerialPort &tempSerial)
{
    int i=1;
    while(i)
    {
     //  tempSerial.writeBuffer(tempSerial.txpackage);
    }
}

#endif //ALLIANCE2020_CV_GROUP_INFANTRYTHREADS_H
