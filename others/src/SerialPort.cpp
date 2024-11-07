#include "../include/SerialPort.h"
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


SerialPort::SerialPort(const char *dev_name) {
    COMBAT_PRINT2("serialID",dev_name)
    fd=open(dev_name,O_RDWR | O_NOCTTY | O_NDELAY);
    //while(fd == -1);
    fcntl(fd, F_SETFL, 0);
    struct termios port_settings;
    cfmakeraw(&port_settings);                            //设置为原始模式,所有输入都按字节处理,实测没有这步配置,FF有时会读成7F甚至5F
    cfsetispeed(&port_settings, B115200);                 //输入波特率
    cfsetospeed(&port_settings, B115200);                 //输出波特率
    //设置数据流控制
    port_settings.c_cflag &= ~PARENB;                     //允许输出产生奇偶信息以及输入的奇偶校验（启用同位产生与侦测）
    port_settings.c_cflag &= ~CSTOPB;                     //
    port_settings.c_cflag &= ~CSIZE;
    port_settings.c_cflag |= CS8;
    port_settings.c_cflag |=(CLOCAL | CREAD);             //保证程序不会占用串口,使得能够从串口中读取输入数据
    tcsetattr(fd, TCSANOW, &port_settings);
}



bool SerialPort::readBuffer(Rxpackage &tempRX) {
    unsigned char rcv_buf[100];
    read(fd,rcv_buf, sizeof(tempRX));
    rxpackage.frameHead=rcv_buf[0];
    rxpackage.pitch= short((rcv_buf[2] << 8|rcv_buf[1]));
   //  rxpackage.pitch |=rcv_buf[1];
   rxpackage.yaw= short((rcv_buf[4] << 8|rcv_buf[3]));
    //rxpackage.yaw=rcv_buf[3];
    rxpackage.roll=rcv_buf[5];
    rxpackage.speed=rcv_buf[7];
    rxpackage.color=rcv_buf[8];
    rxpackage.frameTail=rcv_buf[9];
//    std::cout<<sizeof(tempRX);
//    for(int i=0;i<sizeof(tempRX);i++)
//    {
//        printf("  %02X",rcv_buf[i]);
//    }
//    std::cout<<std::endl;
  //  memcpy(&tempRX,rcv_buf, 10);
    return 1;
}

bool SerialPort::writeBuffer(Tx_package &txTemp) 
{
    unsigned char t_buf[100];
    t_buf[0]=tx_package.frameHead;
    t_buf[1]=static_cast<uint8_t>(tx_package.pitch & 0xFF);
    t_buf[2]=static_cast<uint8_t>((tx_package.pitch >> 8) & 0xFF);
    t_buf[3]=static_cast<uint8_t>(tx_package.yaw & 0xFF);
    t_buf[4]=static_cast<uint8_t>((tx_package.yaw >> 8) & 0xFF);
    t_buf[5]=tx_package.shot;
    t_buf[6]=tx_package.frameTail;
//    memcpy(t_buf,&txTemp, 7);//mempcpy复制内存
//    std::cout<<sizeof(txTemp);
//    for(int i=0;i< sizeof(txTemp);i++)
//    {
//        printf("  %02X",t_buf[i]);
//    }
//    std::cout<<"done"<<std::endl;
    write(fd,t_buf,7);
    //std::cout<<sizeof(txTemp)<<std::endl;
   /*for(int i=0;i<7;i++)
 {

        std::cout<<t_buf[i]<<std::endl;
    }*/
    
    return 1;
}

void SerialPort::closeSerial() {
    close(fd);
}