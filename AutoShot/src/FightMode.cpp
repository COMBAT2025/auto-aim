#include "FightMode.h"
#include "Combat2024_AllTeam.h"
#include <thread>
#include "Setter.h"
#include "MindVisionCamera.h"
#include "SerialPort.h"
#include "HEROTHREADS.h"
#include "INFANTRYTHREADS.h"
#include "ENGINEERTHREADS.h"
#include "DRONETHREADS.h"
#include "SENTRYTHREADS.h"

void FightMode::heroFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial) {
    COMBAT_PRINT("I am  the hero of Combat !")
    std::thread readThread(heroReadSerialport,std::ref(tempSerial));
    std::thread videoDataThread(heroVideoDataSave);
    std::thread imageDealThread(heroImageDeal,std::ref(tempSetter),std::ref(tempCamera),std::ref(tempSerial));
    readThread.join();
    videoDataThread.join();
    imageDealThread.join();

}

void FightMode::engineerFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial) {
    COMBAT_PRINT("I am  the engineer of Combat !")
    std::thread readThread(engineerReadSerialport,std::ref(tempSerial));
    std::thread videoDataThread(engineerVideoDataSave);
    std::thread imageDealThread(engineerImageDeal,std::ref(tempSetter),std::ref(tempCamera),std::ref(tempSerial));
    readThread.join();
    videoDataThread.join();
    imageDealThread.join();

}

void FightMode::infantryFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial) {
    COMBAT_PRINT("I am  the infantry of Combat !")
    std::thread readThread(infantryReadSerialport,std::ref(tempSerial));
    std::thread videoDataThread(infantryVideoDataSave);
    std::thread imageDealThread(infantryImageDeal,std::ref(tempSetter),std::ref(tempCamera),std::ref(tempSerial));
    std::thread writeThread(infantryWriteSerialport,std::ref(tempSerial));
    readThread.join();
    imageDealThread.join();
    videoDataThread.join();
    writeThread.join();
}

void FightMode::droneFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial) {
    COMBAT_PRINT("I am  the infantry of Combat !")
    std::thread readThread(droneReadSerialport,std::ref(tempSerial));
    std::thread videoDataThread(droneVideoDataSave);
    std::thread imageDealThread(droneImageDeal,std::ref(tempSetter),std::ref(tempCamera),std::ref(tempSerial));
    readThread.join();
    imageDealThread.join();
    videoDataThread.join();
}

void FightMode::sentryFight(Setter &tempSetter,MindVisionCamera &tempCamera,SerialPort &tempSerial) {
    COMBAT_PRINT("I am  the sentry of Combat !")
    std::thread readThread(sentryReadSerialport1,std::ref(tempSerial));
    std::thread videoDataThread(sentryVideoDataSave);
    std::thread imageDealThread(sentryImageDeal1,std::ref(tempSetter),std::ref(tempCamera),std::ref(tempSerial));
    readThread.join();
    imageDealThread.join();
    //videoDataThread.join();

}