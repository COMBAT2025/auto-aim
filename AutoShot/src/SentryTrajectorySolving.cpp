#include "SentryTrajectorySolving.h"
#include "TimerBase.h"
#include "Filtering.h"
#define disCOE 0.0175                   //单位是米
turnAngle solveEquation(float speed,cv::Mat tvec,float pitch,float yaw)
{   //所有单位必须为标准国际单位
    double a,b,c,delta,z,y,x,solve1,solve2;//可以在速度上加上哨兵的移动速度
    double pitchRad,yawRad;
    turnAngle result;
    Filtering filtering;
    pitchRad=pitch*pi/180.0;
    yawRad=yaw*pi/180.0;//角度化为弧度
    //单位从毫米变成米
    x=tvec.at<double>(0)/1000 + 0.07;//手动补偿测距偏差
    y=tvec.at<double>(1)/1000  ;//这里加的是摄像头与枪管的距离和误差
    z=tvec.at<double>(2)/1000;
    //z =filtering.loop(z);
    printf("x上距离%f\n",float(x));
    printf("y上距离%f\n",float(y));
    printf("z上距离%f\n",float(z));
    y=tvec.at<double>(1)/1000 - 0.5*gra*(z/speed)*(z/speed);
    std::cout<<"y轴现为:"<<y<<std::endl;
    result.yawAngle=short(atan(x/z)*8192/pi/2);//Yaw轴直接算出来,乘8192/360
//    std::cout<<"Yaw轴转"<<atan(x/z)*180/pi<<"度"<<std::endl;
//    std::cout<<"Yaw轴转"<<atan(x/z)*8192/pi/2<<"码"<<std::endl;
  
  
   a=gra*(exp(k_air*z)-1)/(2*speed*k_air);
    b=(exp(k_air*z)-1)/k_air;
    c=-y+gra*(exp(k_air*z)-1)/(2*k_air*speed);
    delta=b*b-4*a*c;
    solve1=(-b+sqrt(delta))/(2*a);//两个解，一个大于零一个小于零
    solve2=(-b-sqrt(delta))/(2*a);
    // std::cout<<"Pitch轴转"<<result.pitchAngle<<"码"<<std::endl;


    result.pitchAngle=short(((atan(y/z)/*-pitchRad*/)*8192/pi/2/*-1500*/));
    //std::cout<<"Pitch轴绝对坐标应该是"<<atan(y/z)*180/pi<<"度"<<std::endl;
    return result;
}

// void outputShow(cv::Mat &src,cv::Mat tvec,turnAngle result)
// {
//     cv::line(src,cv::Point(960,0),cv::Point(960,1200),Y,1,8,0);
//     cv::line(src,cv::Point(0,600),cv::Point(1920,600),Y,1,8,0);
// }
void outputShow(cv::Mat &src,cv::Mat tvec,turnAngle result)
{
    cv::line(src,cv::Point(480,0),cv::Point(480,600),COLOR_Y,1,8,0);
    cv::line(src,cv::Point(0,300),cv::Point(960,300),COLOR_Y,1,8,0);
}
int forSongziwu(int codeNum,float YawAngle,float dis)//这个函数为了纪念宋子午在省赛中的卓越贡献
{                             //  Yaw是弧度
    YawAngle=YawAngle/180*PI;
    cv::Point2f me,enemy;
    cv::Point2f column1(2.3,-0.0745);
    cv::Point2f column2(2.3,2.1745);
    me.x=0;
    me.y=2.1-0.0175*codeNum;
    enemy.x=dis/1000*cos(YawAngle);
    enemy.y=me.y-dis/1000*sin(YawAngle);
    //std::cout<<"我的y："<<me.y<<std::endl;
    if(enemy.x<=3.0) return 2;
    cv::Point2f vec1=enemy-me;
    cv::Point2f vec2=column1-me;
    cv::Point2f vec3=column2-me;
//    std::cout<<"vec1："<<vec1<<std::endl;
//    std::cout<<"vec2："<<vec2<<std::endl;
//    std::cout<<"vec3："<<vec3<<std::endl;
    float res1,res2;
    res1=vec1.x*vec2.y-vec1.y*vec2.x;
    res2=vec3.x*vec1.y-vec3.y*vec1.x;
    if(res1*res2>0)
        return 1;
    else
        return 2;
}