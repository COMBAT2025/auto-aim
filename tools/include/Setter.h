#ifndef SETTER_H
#define SETTER_H

#include "Combat2024_AllTeam.h"


struct CameraDistortion{
    cv::Mat CameraMatrix;
    cv::Mat DistCoeffs;
};

struct PredictModel {
    int dynamParams;
    int measureParams;
    cv::Mat transMat;
};


class Setter {

public:
    Setter();

    int  Model;



    struct SerialPort{
        std::string SerialPath;
        bool isSerialOnline = false;
        bool isSerialDebugIndependently = true;
        bool isShowSerialWriteDataMainThread = false;
        bool isShowSerialWriteDataIndependentThread = false;
        bool isShowSerialReadDataMainThread = false;
        bool isShowSerialReadDataIndependentThread = false;
        bool isSerialDebug = false;
    };



    struct Threshold{
        int Threshold4Gray2Binary = 150;
        int Threshold4Gray2BinaryPowerTrigger = 100;
        int cvWaitKeyTime = 30;
    };


    struct FrameSource{
        std::string VideoPath;
        std::string CameraParameterPath;
    };


    struct CalibratorPara{
        std::string Calibration;                           //标定照片路径
        cv::Size Rectangle;                                //每个格子大小 /mm
        cv::Size CornersMatrix;                            //格子个数
        std::string CalibrationResultPath;
        std::string CalibrationPointPath;
        std::string SAVE_IMAGE_PATH;

    };


    struct ArmourPara{
        float theMaximumdifference42Leds=3;
        float theMaximumDistance42Leds=4.5;
        float theMinimumDistance42Leds;
        int theMinmumSize4aContours=20;
        float supLengthOverWidth=7;
        float infLengthOverWidth=1.5;
        int threshold4Gray2Binary4Red=/*100*/90;
        int threshold4Gray2Binary4Blue=110;
        int Threshold4Gray2BinaryPowerTrigger=100;
        int cvWaitKeyTime=30;
        float minAngle=20;
        bool whoIam=RED;
        bool isShowGrayImage=true;
        bool isShowOriginImage = true;
        bool isShowFinallyImage = true;                    // A BGR image whit Drawn information.
    };

    struct CameraSize{
        cv::Size shootingImageSize;
        cv::Size resolution;
    };

    struct SvmPara{
        std::string setType="SVM::C_SVC";
        std::string setKernel="SVM::LINEAR";
        int setDegree=0;
        int iterMaxTimes=1000;
        int learnRatio=0.01;
        int setGamma=1;
        int setCoef0=0;
        int setC=1;
        int setNu=0;
        int setP=0;
    };

    struct CNNPara{
        int LinearGain;
        float Dyaw;
        bool isUsingCNN = false;
        std::string CNN;
    };

    struct DataSave{
        std::string DataSavePath;
        int DataSaveInterval = 0;
    };

    struct VideoSave{
        std::string VideoSavePath;
        bool isSaveOriginal = false;
        bool isSaveResultVideo = false;
    };


    struct ArmourSize
    {
        float width=0;
        float len=0;
    };

    struct TrajectoryAnalyz{
        double gravity;
        double coefficient_K;
    };

                      //ÃèÊöÐèÒªÔ€²âµÄÄ£ÐÍ

    /// short int: 65535
    /// int: 2^32
    /// float: 4KB
    /// double: 8KB
    /// bool: 1KB
    /// 1. Take using sint when Calculating into Consideration



    SerialPort       serialPort;                  //王海龙
    Threshold        threshold;                   //阈值
    FrameSource      framesource;                 //frame来源，读摄像头还是读视频
    CalibratorPara   calibratorPara;              //标定图片来源，角点个数，矩形大小
    ArmourPara       armourPara;                  
    CameraDistortion cameraDistortion;            //相机畸变系数和矩阵
    CameraSize       cameraSize;                  //摄像头拍照尺寸 默认1920*1200
    SvmPara          svmPara;                     //SVM参数
    CNNPara          cnnPara;                     //CNN参数
    DataSave         dataSave;                    //日志保存
    VideoSave        videoSave;                   //视频保存
    ArmourSize       allsize[3];                  //大小装甲、大符尺寸
    TrajectoryAnalyz trajectoryAnalyz;            //重力加速度，空气阻力系数K
    PredictModel     AnglePModelY;                 //角度模型
    PredictModel     AnglePModelP;                 //角度模型
    PredictModel     AnglePModelF;                 //平滑模型
};




#endif //SETTER_H
