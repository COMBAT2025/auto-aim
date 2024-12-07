#ifndef SENTRYTHREADS_H
#define SENTRYTHREADS_H

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
#include "SentryTrajectorySolving.h"
#include "MathTool.h"
#include "EasyFind.h"
#include<pid.h>


//SENTRY_thread 串口接收
void sentryReadSerialport1(SerialPort &tempSerial)
{
    int i=1;
    while(i)
    {
        tempSerial.readBuffer(tempSerial.rxpackage);
    }
}



//SENTRY_thread 装甲识别、解算、预测、发送
int sentryImageDeal1(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial)
{   
    PNPSolver pnpSolver(tempSetter.allsize);
    ArmourFinder armourFinder(tempSetter.armourPara,tempSetter.armourPara.whoIam);
    cv::Mat_<double> tvec(3,1),rvec(3,1);
    int strikeTarget,lastTarget,kcount=0,tCount=0;
    int tempCount=0;
    cv::Mat originFrame;
    short preCodeY=0;
    float imageYaw,totalYaw,imagePitch,totalPitch,dx=0,dy=0,fdx=0,fdy=0;//卡尔曼滤波的参数
    struct turnAngle result;
    float bulletSpeed,nowPitch=0.0,nowYaw=0.0,moveSpeed=0.0;//

    // 创建 PID 控制器实例
    PID_TypeDef PID_Pitch;    
    PID_TypeDef PID_Yaw;

    // 初始化 PID 参数
    Pid_Set(&PID_Pitch, 0.05f, 0.0f, 0.1f, 50.0f, 100, 200, 200, 1.0f, 0.0f, 0.01f, 0);
//  Pid_Set(&PID_Yaw,  0.03f, 0.0f, 0.3f, 50.0f, 100, 200, 200, 1.0f, 0.0f, 0.01f, 0);
    Pid_Set(&PID_Yaw,  0.05f, 0.0f,0.9f, 50.0f, 100, 200, 200, 1.0f, 0.0f, 0.01f, 0);


    Predict anglePreY(tempSetter.AnglePModelY,1);
    Predict anglePreP(tempSetter.AnglePModelP,1);
    Predict anglePreF(tempSetter.AnglePModelF,2);
    static cv::Mat measurementY = cv::Mat::zeros(1,1,CV_32F);
    static cv::Mat measurementP = cv::Mat::zeros(1,1,CV_32F);
    measurementY.at<float>(0)=0.0;
    measurementP.at<float>(0)=0.0;
    timer Timer;
    int i=1;
    int c = 0; // 声明并初始化计数变量
    int *cPtr = &c; // 声明指向计数变量的指针
    tempSerial.tx_package.shot = 0;
    while(i) {
        if(c>=50){
        *cPtr = 0;
        tempSerial.tx_package.shot = 0;
        }
        bulletSpeed = tempSerial.rxpackage.speed;
        armourFinder.findWho = tempSerial.rxpackage.color;//敌人红是1，蓝是0
        armourFinder.findWho = 1;
        //std::cout<<tempSerial.rxpackage.speed<<std::endl;
         nowPitch =(tempSerial.rxpackage.pitch-7503)/ 8192 *360/**360+ 5.27*/;
         nowYaw = -(tempSerial.rxpackage.yaw-4095) / 8192 *360/** 180*/;//根据电控发的信息


        std::cout<<"电控发的P："<<tempSerial.rxpackage.pitch<<std::endl;
        std::cout<<"电控发的Y:"<<tempSerial.rxpackage.yaw<<std::endl;
        //std::cout<<"pitch: "<<tempSerial.rxpackage.pitch<<std::endl;
         //std::cout<<"nowpitch: "<<nowPitch<<std::endl;
        //std::cout<<"nowyaw: "<<nowYaw<<std::endl;
        //moveSpeed = tempSerial.rxpackage.speed;
        tempCamera >> originFrame;
        cv::flip(originFrame, originFrame, -1);
        videoSave=originFrame;
        if (tempSetter.armourPara.isShowOriginImage == TRUE) {
            cv::imshow("origin", originFrame);
            cv::waitKey(1);
        }
        std::vector<Armour> armours;
        int cmode=0;
        armours = armourFinder.findArmour(tempSetter,originFrame,cmode);

        //当前装甲板相对于平行摄像头偏移的角度
       // std::cout << armourFinder.angle_z <<std::endl;

        TargetDecision targetDecision(armours);
        strikeTarget = targetDecision.PrintTarget();
        //std::cout<<strikeTarget<<std::endl;
        Timer.showOnImage(originFrame);
        if (strikeTarget != -1) {
            kcount = 0;
            lastTarget = strikeTarget;
            tempSerial.tx_package.shot = 1;
            // 这里是角度卡尔曼
            //82.294   28.32
            // imageYaw = 43.84 / 800 * (armours[strikeTarget].center.x - 400.0) * 0.622;
            //imagePitch = 28.32 / 600 * (armours[strikeTarget].center.y - 300.0) * 0.52205;
            imageYaw = 35.94 / 960 * (armours[strikeTarget].center.x - 480.0) * 0.200333;

            /****************************************
            FOV = 2*arctan(0.5*width_pic/focal(piexl))         
            光学倍率 = CCD芯片尺寸/FOV
            ****************************************/

            //imagePitch = 28.32 / 600 * (armours[strikeTarget].center.y - 300.0) * 0.52205;
            totalYaw = nowYaw + imageYaw;
            totalPitch = nowPitch + imagePitch;
            measurementY.at<float>(0)=totalYaw;
            measurementP.at<float>(0)=totalPitch;
            anglePreY.correct(measurementY);
            anglePreP.correct(measurementP);
            cv::Mat preY = anglePreY.predict();
            cv::Mat preP = anglePreP.predict();
            cv::Point2f preCenter(armours[strikeTarget].center);
//          float tempx=preY.at<float>(0)-totalYaw;
//          tempx=tempx*20+totalYaw;
//          tempx=anglePreF.smooth(tempx);
//          dx=(tempx-totalYaw)*800/43.84;
            dx = 7.5*(preY.at<float>(0) - totalYaw) * 960 / 35.94;
            
            //dy=10.0*(preP.at<float>(0)-totalPitch)*600/28.32;
             preCenter.x = armours[strikeTarget].center.x + dx;
            //std::cout<< "dx:"<<dx<<std::endl;

            //没有预测pitch，故dy = 0
            preCenter.y=armours[strikeTarget].center.y + dy;

           // std::cout<<armours[strikeTarget].center<<std::endl;
            cv::circle(originFrame, preCenter, 10, COLOR_G, 3, 8);
            std::vector<cv::Point2f> preObj;
            preObj.emplace_back(
                    cv::Point2f(armours[strikeTarget].concors[0].x + dx, armours[strikeTarget].concors[0].y + dy));
            preObj.emplace_back(
                    cv::Point2f(armours[strikeTarget].concors[1].x + dx, armours[strikeTarget].concors[1].y + dy));
            preObj.emplace_back(
                    cv::Point2f(armours[strikeTarget].concors[2].x + dx, armours[strikeTarget].concors[2].y + dy));
            preObj.emplace_back(
                    cv::Point2f(armours[strikeTarget].concors[3].x + dx, armours[strikeTarget].concors[3].y + dy));
            cv::line(originFrame, preObj[0], preObj[1], COLOR_W, 1, 8);
            cv::line(originFrame, preObj[1], preObj[2], COLOR_W, 1, 8);
            cv::line(originFrame, preObj[2], preObj[3], COLOR_W, 1, 8);
            cv::line(originFrame, preObj[0], preObj[3], COLOR_W, 1, 8);
            //这个下面改变PnP可以直接选直瞄还是预测
            //std::cout<<cmode<<std::endl;
            //tvec = pnpSolver.armourDistance(armours[strikeTarget].concors,cmode,tempSetter.cameraDistortion,rvec);   
           tvec = pnpSolver.armourDistance(preObj, cmode, tempSetter.cameraDistortion, rvec);
            //printf("x上距离%f\n",float(tvec.at<double>(0)));
//            printf("y上距离%f\n",float(tvec.at<double>(1)));
//            printf("z上距离%f\n",float(tvec.at<double>(2)));
            std::vector<cv::Point3f> bestArmours = armourFinder.getArmours(tvec, armourFinder.angle_z);
            std::cout << bestArmours <<std::endl;
            // std::vector<cv::Point2f> rightArmour;
            // std::vector<cv::Point2f> leftArmour;

            // std::vector<cv::Point3f> pointVector1 = { threeArmours[1] };
            // std::vector<cv::Point3f> pointVector2 = { threeArmours[2] };

            // rightArmour = armourFinder.world2pixel(pointVector1,tempSetter.cameraDistortion);
            // leftArmour = armourFinder.world2pixel(pointVector2,tempSetter.cameraDistortion);

            // std::cout << rightArmour <<std::endl;
            // std::cout << leftArmour <<std::endl;
            // cv::circle(originFrame, rightArmour[0], 10, COLOR_G, 3, 8);
            // cv::circle(originFrame, leftArmour[0], 10, COLOR_G, 3, 8);
            double distance = sqrt(tvec.at<double>(0) * tvec.at<double>(0) +
                                   tvec.at<double>(1) * tvec.at<double>(1) + tvec.at<double>(2) * tvec.at<double>(2));
            tempSerial.t_package.distance = short(distance/100);
            // double转short  2081    6108//6005/22388
            //6845 horizon往下增加往上减少
            char dis[100];
            sprintf(dis, "%f", distance);
            cv::putText(originFrame, dis, cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(255, 0, 255), 2,
                        0);
            result = solveEquation//(25.0,tvec, 30.0,0.0);
                    (15, tvec, nowPitch, nowYaw);
            //弹道解算,接下来需要把输出的值变为串口发送需要的类型
            if ((preCodeY > 0 && result.yawAngle < 0)||(preCodeY < 0 && result.yawAngle > 0))
                if (preCodeY != 10000) {
                    tempSerial.tx_package.yaw = (result.yawAngle + preCodeY) / 2;
                }

            float targetPitch = result.pitchAngle + tempSerial.rxpackage.pitch; 
            float targetYaw   = result.yawAngle + tempSerial.rxpackage.yaw;
            float pitchOutput = Pid_Calc(&PID_Pitch, tempSerial.rxpackage.pitch, targetPitch);
            float yawOutput = Pid_Calc(&PID_Yaw, tempSerial.rxpackage.yaw, targetYaw);


            tempSerial.tx_package.pitch = pitchOutput/*result.pitchAngle*/;
            tempSerial.tx_package.yaw = yawOutput ;
             std::cout<<"原本Pitch轴转"<<result.pitchAngle<<"码"<<std::endl;
              std::cout << "原本Yaw轴转" << result.yawAngle << "码" << std::endl;
              std::cout<<"Pitch轴转"<<tempSerial.tx_package.pitch<<"码"<<std::endl;
              std::cout << "Yaw轴转" << tempSerial.tx_package.yaw << "码" << std::endl;
            preCodeY = tempSerial.tx_package.yaw;
           
            if (result.pitchAngle <= 50 && result.pitchAngle >= -50) {
                if (result.yawAngle <= 50 && result.yawAngle >= -50) {
                    tempSerial.tx_package.shot = forSongziwu(bulletSpeed,nowYaw,distance);//不打墙后面的人
                     //printf("%f",distance);
                    if(distance<=6000)
                    {  
                        tempSerial.tx_package.shot=2;
                        //std::cout<<"开火"<<std::endl;
                    }
                    
                    /*if(tempSerial.tx_package.shot==2)
                    std::cout<<"开火"<<std::endl;*/
                }
            }//判断是否发射
        } 
        else {(*cPtr)++;
        tempSerial.tx_package.pitch = 0;
        tempSerial.tx_package.yaw = 0;
        }
        /*else {
            if (lastTarget != -1) {//判断敌人从哪里消失
                if (preCodeY < 0) {
                    tempSerial.t_package.enemy = 1;
                } else tempSerial.t_package.enemy = 2;
            }
            tempSerial.t_package.shot = 0;
            kcount++;
            if (kcount >= 50) {
                //anglePreY.myInit(tempSetter.AnglePModelY);
                //anglePreP.myInit(tempSetter.AnglePModelP);
                preCodeY = 10000;
            }
        }*/ //巡航





        tempSerial.writeBuffer(tempSerial.tx_package);
        if (tempSetter.armourPara.isShowFinallyImage == TRUE) {
            outputShow(originFrame, tvec, result);
            cv::imshow("final", originFrame);
            cv::waitKey(1);
        }

    }
    return 0;
}


//SENTRY_thread 5 视频录制、日志撰写
int sentryVideoDataSave()
{
    cv::VideoWriter writer(getTimePictureC(),cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 50, cv::Size(800, 600));
    while(1)
    {
        writer << videoSave;
    }
}

#endif //SENTRYTHREADS_H
