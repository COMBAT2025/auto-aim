#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "Combat2024_AllTeam.h"

class SerialPort {
public:

struct Tx_package    //发送包                 //float 4字节 char 1字节 int 4字节
    {
        char   frameHead=0XFF;                     
        short  pitch=0;                    //需要旋转的pitch角度，值为原角度乘10
        short  yaw=0;                      //需要旋转的yaw角度，值为原角度乘10   
        char   shot=0;   
        char   frameTail=0XFE;
    };


    struct T_package
    {
      char enemy=2;
      short distance;
       char   shot=0;                       //状态位，0巡航，1发现敌人，2射击
        char   model;                      //是否处于巡航状态,1为巡航，0为跟随
                               
    };



    struct Rxpackage    //接受包
    {

        char   frameHead=0xFF; 
        float    pitch;   //当前pitch角度，下为正，大于9000的为负数
        float yaw;    //当前yaw角度，右为正，大于9000的为负数
        short roll;     //当前row角度，右为正，大于9000的为负数
        short  speed;                 //子弹速度
        char   color;                 //当前颜色          
        char   frameTail=0xFE;                  
    };

    int fd;

    Rxpackage rxpackage;
    //Txpackage tx_package;
    T_package t_package;
    Tx_package tx_package;

    SerialPort(const char * dev_name);
    void closeSerial();
    bool readBuffer(Rxpackage &tempRX);
    bool writeBuffer(Tx_package &txTemp);

};


#endif //ALLIANCE2020_CV_GROUP_SERIALPORT_H