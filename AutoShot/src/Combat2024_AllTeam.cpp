#include "Combat2024_AllTeam.h"

cv::Mat videoSave;                            //稳哥交代全局变量这样定义，电控都这么写
char colorFlag=1;                    //1 red  0 blue
unsigned char modeFlag=0;                     //1 高速  2 低速  3高速小陀螺  4低速小脱落
unsigned char lastModeFlag=0;
