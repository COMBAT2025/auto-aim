#ifndef TARGETDESICION_H
#define TARGETDESICION_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <MathTool.h>
#include "Armour.h"
#include "ArmourFinder.h"

class TargetDecision
{
public:
    TargetDecision();
    TargetDecision(std::vector<Armour> enemyArmours);
    int PrintTarget();

private:
    std::vector<float> armorDistance;
    int armorPoints[20];
    int target;
    int count;
    int number;
};


#endif //TARGETDESICION_H