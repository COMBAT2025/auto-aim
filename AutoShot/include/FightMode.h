#ifndef BATTLEMODE_H
#define BATTLEMODE_H

#include "Setter.h"
#include "MindVisionCamera.h"
#include "SerialPort.h"



class FightMode {
public:
    void heroFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial);
    void engineerFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial);
    void infantryFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial);
    void droneFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial);
    void sentryFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial);

};


#endif //BATTLEMODE_H
