#ifndef TIMERBASE_H
#define TIMERBASE_H

#include "Combat2024_AllTeam.h"
#include <time.h>

inline std::string getTimePictureC() {
    time_t TimeValue;
    time (&TimeValue);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "../others/video/%Y-%m-%d_%H_%M_%S.avi",localtime(&TimeValue));
    return tmp;
}


class timer
{
private:
    double start,finish;
    int numT=0;    //运行周期

public:
    timer();
    std::string showTime();
    void showOnImage(cv::Mat image);
    void update();
    void clear();
    void functiontime();
    
};


#endif //TIMERBASE_H
