#ifndef EASYFIND_H
#define EASYFIND_H

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

class EasyFind {
public:
    EasyFind();
    void findme(cv::Mat &src,SerialPort &tempSerial,char whoIAm,std::vector<Armour> &Armors);

private:
    cv::Mat gray;
    std::vector<std::vector<cv::Point>> allContours;
    std::vector<Marker> markerGroup;
    void adjustOrder(Marker m1,Marker m2,cv::Point2f points[]);
    Svm svm;
};


#endif //EASYFIND_H
