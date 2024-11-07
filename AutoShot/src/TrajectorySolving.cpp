#include "TrajectorySolving.h"

//setter
float pitchAngle=0;
float yawAngle=0;
float initVelocity=20.0;                                                                                                //meter per second
float resistanceCoeffience=0.001;  //空气阻力系数                                                                                     //resistance from air
float bulletMass=0.003;
float gravityAcceleration=9.8;

//value in calculating
float horizontalDistance=0;
float verticalDistance=0;
float extremeAngle=0;                                                                                                   //angle with highest hitted height

float angleBinarySearch(int minAngle,int maxAngle)
{
  //std::cout<<"最小最大值"<<minAngle<<"   "<<maxAngle<<std::endl;
    if(minAngle>=maxAngle)  return 1e9;
    float angle=(minAngle+maxAngle)/2/1000.0*1.5707;
    float horizontalVolecity=initVelocity*cos(angle);
    float verticalVolecity=initVelocity*sin(angle);
    float flightTime=(exp(resistanceCoeffience*horizontalDistance/bulletMass)-1)/(resistanceCoeffience*initVelocity/bulletMass);        //time in air
    float hittedHeight=verticalVolecity*flightTime-gravityAcceleration/2*pow(flightTime,2);                                             //finally hitted position
    if(angle>extremeAngle)
    {
        if(hittedHeight-verticalDistance<0.1&&hittedHeight-verticalDistance>-0.1)
        {
            return angle;
        }
        else
        {
            if(hittedHeight>verticalDistance)   return angleBinarySearch((minAngle+maxAngle)/2+1,maxAngle);
            else                                return angleBinarySearch(minAngle,(minAngle+maxAngle)/2-1);
        }
    }
    else
    {
        if(hittedHeight-verticalDistance<0.1&&hittedHeight-verticalDistance>-0.1)
        {
            return angle;
        }
        else
        {
            if(hittedHeight>verticalDistance)   return angleBinarySearch(minAngle,(minAngle+maxAngle)/2-1);
            else                                return angleBinarySearch((minAngle+maxAngle)/2+1,maxAngle);
        }
    }
}

panTiltZoomCameraOffsetAngle trajectoryResolving(cv::Mat transitionVector,float pitchAngle,float yawAngle,float initVelocity)
{
    float xCamera=transitionVector.at<double>(0);                       //m       x=z
    float yCamera=transitionVector.at<double>(1);                       //m       y=-y                                                  //coordinate by camera
    float zCamera=transitionVector.at<double>(2);                       //m       z=x
    //xCamera=4.0;
    //std::cout<<"xyz"<<xCamera<<" "<<yCamera<<" "<<zCamera<<std::endl;
    //std::cout<<transitionVector<<std::endl;
   pitchAngle=pitchAngle*PI/180.0;
   yawAngle=yawAngle*PI/180.0;//云台角度化为弧度
    //std::cout<<"电控pitch、yaw"<<pitchAngle<<" "<<yawAngle<<std::endl;
    float xIntersection=xCamera*cos(-pitchAngle)-yCamera*sin(-pitchAngle);                                                                //coordinate by intersection
    float yIntersection=xCamera*sin(-pitchAngle)+yCamera*cos(-pitchAngle);
    float zIntersection=zCamera;
    //std::cout<<"车身"<<xIntersection<<" "<<yIntersection<<" "<<zIntersection<<std::endl;
    horizontalDistance=sqrt(pow(xIntersection,2)+pow(zIntersection,2));
    verticalDistance=yIntersection;
    //std::cout<<"水平面"<<horizontalDistance<<" "<<verticalDistance<<std::endl;
    extremeAngle=atan((resistanceCoeffience*pow(initVelocity,2))/
                      (gravityAcceleration*(bulletMass*exp(resistanceCoeffience*horizontalDistance/bulletMass)-bulletMass)));       //highest hitted height occurs with this angle
    //COMBAT_PRINT2("extremeAngle",extremeAngle)
    panTiltZoomCameraOffsetAngle result;
    result.yawAngle=atan(zIntersection/xIntersection);
    float resultAngle1=angleBinarySearch(-500,(int)(extremeAngle/1.5707*1000));
    float resultAngle2=angleBinarySearch((int)(extremeAngle/1.5707*1000)+1,1000);
    //二分法有问题
    //  std::cout<<"resualtAngle1:"<<resultAngle1<<"  "<<"resultAngle2:"<<resultAngle2<<std::endl;
    //  std::cout<<result.pitchAngle<<std::endl;

    result.pitchAngle=(resultAngle1<resultAngle2?resultAngle1:resultAngle2)-pitchAngle;                                                 //canon//    for (int i = -500; i < 1000; i++)
    COMBAT_PRINT(result.pitchAngle);
    if(result.pitchAngle>9e8)   result.pitchAngle=0;
//    {
//        float angle = i * 1.5707 / 1000.0;
//        float horizontalVolecity = initVelocity * cos(angle);
//        float verticalVolecity = initVelocity * sin(angle);
//        float flightTime = (exp(resistanceCoeffience * horizontalDistance / bulletMass) - 1) / (resistanceCoeffience * initVelocity / bulletMass);
//        float hittedHeight = verticalVolecity * flightTime - gravityAcceleration / 2 * pow(flightTime, 2);
//
//
//        if (hittedHeight - verticalDistance < 0.001&&hittedHeight - verticalDistance>-0.001)
//        {
//            std::cout<<i<<" "<<verticalVolecity<<" "<<hittedHeight - verticalDistance<<std::endl;
//            result.pitchAngle = angle;
//            break;
//        }
//    }
    //cv::waitKey(0);
result.yawAngle=result.yawAngle*180.0/PI;
    if(result.yawAngle>15||result.yawAngle<-15) result.yawAngle=0;

    result.pitchAngle=result.pitchAngle*180.0/PI;
//    std::cout<<"result"<<result.pitchAngle<<" "<<result.yawAngle<<std::endl;
    return result;
}