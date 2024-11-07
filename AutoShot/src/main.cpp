#include "Combat2024_AllTeam.h"
#include "Matrix.h"
#include "Setter.h"
#include "Calibrator.h"
#include "SerialPort.h"
#include "MindVisionCamera.h"
#include <thread>
#include <FightMode.h>
#include "ArmourFinder.h"
#include "MathTool.h"



int main(int argv,char **argc) {


    COMBAT_PRINT2("The Current path",argc[0])
    Setter setter;
    MindVisionCamera industryCamera;
    industryCamera.SetExposureTime(3);
     //cv::Size resolution = industryCamera.ReadResolution();
   // std::cout << "Camera Resolution: " << resolution.width << "x" << resolution.height << std::endl;
  //  industryCamera.SetResolution(cv::Size(1920,1200),cv::Size(0,0));
    industryCamera.SetResolution(cv::Size(960,600),cv::Size(0,0));
    COMBAT_PRINT("hello Combat");
    SerialPort serialPort(setter.serialPort.SerialPath.c_str());
    if(argv == 1){
        COMBAT_PRINT("Combat_Fight!");
        FightMode CombatFight;
        if(setter.Model==HERO)
            CombatFight.heroFight(setter,industryCamera,serialPort);
        else if(setter.Model==ENGINEER)
            CombatFight.engineerFight(setter,industryCamera,serialPort);
        else if(setter.Model==INFANTRY)
            CombatFight.infantryFight(setter,industryCamera,serialPort);
        else if(setter.Model==DRONE)
            CombatFight.droneFight(setter,industryCamera,serialPort);
        else if(setter.Model==SENTRY)
            CombatFight.sentryFight(setter,industryCamera,serialPort);
    }else if(argv == 2 && argc[1][0] == 'c'){
        COMBAT_PRINT("标定")
        //Run in a special mode -- Clibration functionality
        Calibrator cal(setter.calibratorPara,industryCamera);
        //calibrator.save_calibrate_result();
    }else if(argv == 2 && argc[1][0] == 't'){         //Run in a special mode -- Test
        COMBAT_PRINT(3);
        while(1){
            serialPort.t_package.shot=2;
            //std::cin>>serialPort.txpackage.pitch>>serialPort.txpackage.yaw>>serialPort.txpackage.shot;
            serialPort.writeBuffer(serialPort.tx_package);
            serialPort.readBuffer(serialPort.rxpackage);
            //std::cout<<serialPort.rxpackage.bulletSpeed<<std::endl;
            //std::cout<<serialPort.rxpackage.pitch+0<<" "<<serialPort.rxpackage.yaw+0<<std::endl;
//            printf("  %d",serialPort.rxpackage.color);

        }
    }else if(argv == 2 && argc[1][0] == 'm'){
        //movie test mode
    }else{
        std::cout << "Please Select one correct paameter!" << std::endl;
    }

    return 0;
}