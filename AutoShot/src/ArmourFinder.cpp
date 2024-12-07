#include "MathTool.h"
#include "ArmourFinder.h"
#include <iostream>
#include "Setter.h"
#include <vector>
#define BC channels[0]
#define GC channels[1]
#define RC channels[2]


//init
ArmourFinder::ArmourFinder(Setter::ArmourPara &para,bool whoIam)
{
    svm.load("../tools/parameters_lists/SVM.xml");
    maxDifference=para.theMaximumdifference42Leds;
    maxDistance=para.theMaximumDistance42Leds;
    minDistance=para.theMinimumDistance42Leds;
    minContourSize=para.theMinmumSize4aContours;
    supLengthWidth=para.supLengthOverWidth;
    infLengthWith=para.infLengthOverWidth;
    minAngle=para.minAngle;
    isShowGrayImage=para.isShowGrayImage;
    isShowFinallImage=para.isShowFinallyImage;

    if(whoIam==RED)
    {
        thresh4Binary=para.threshold4Gray2Binary4Blue;
        printf("I am red,I need find blue\n");
        findWho=BLUE;
    }
    if(whoIam==BLUE)
    {
        thresh4Binary=para.threshold4Gray2Binary4Red;
        printf("I am blue,I need find red\n");
        findWho=RED;
    }
    if(para.isShowGrayImage)
    {
        cv::namedWindow("Binary",cv::WINDOW_NORMAL);
    }
}

//binary
// void ArmourFinder::imageBinary(Setter &tempSetter,cv::Mat &inPut, cv::Mat &out)
// {
//     std::vector<cv::Mat> channels;
//     cv::split(inPut,channels);
//     if(findWho==RED)
//         out=RC-BC*0.8-GC*0.1;
//     if(findWho==BLUE)
//         out=BC-RC*0.7-GC*0.1;
//     out=out>thresh4Binary;
//     cv::Mat element=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
//     cv::dilate(out,out,element);
//     if(tempSetter.armourPara.isShowGrayImage)
//         cv::imshow("Binary",out);

// }
//以上代码使用RBG三通道来增强红色或蓝色，鲁棒性不高
void ArmourFinder::imageBinary(Setter &tempSetter, cv::Mat &inPut, cv::Mat &out)
{
    cv::Mat hsv;
    cv::cvtColor(inPut, hsv, cv::COLOR_BGR2HSV); // 将输入图像转换为HSV颜色空间

    // 切分HSV通道
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    cv::Mat H = channels[0]; // Hue通道
    cv::Mat S = channels[1]; // Saturation通道
    cv::Mat V = channels[2]; // Value通道

    if (findWho == RED)
    {
        // 对红色进行增强处理
        cv::Mat redEnhanced = (H < 10) | (H > 170); // 在HSV颜色空间中，红色的Hue值在0-10和160-180之间
        out = redEnhanced & (S > 50) & (V > 50); // 同时满足饱和度和亮度要求
    }
    else if (findWho == BLUE)
    {
        // 对蓝色进行增强处理
        cv::Mat blueEnhanced = (H > 90) & (H < 130); // 在HSV颜色空间中，蓝色的Hue值在90-130之间
        out = blueEnhanced & (S > 50) & (V > 50); // 同时满足饱和度和亮度要求
    }

    // 二值化处理
    //out = out > thresh4Binary;
       // 二值化处理，将大于阈值的设置为255，小于等于阈值的设置为0
    cv::threshold(out, out, thresh4Binary, 255, cv::THRESH_BINARY);
     //形态学膨胀 这玩意会影响测距
   cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
   cv::dilate(out, out, element);

    // 如果设置了显示灰度图像的选项，则显示处理后的图像
    if (tempSetter.armourPara.isShowGrayImage)
    {
        cv::imshow("Binary", out);
    }
}

// std::vector<Marker> ArmourFinder::findMark(Setter &tempSetter,cv::Mat &frame)
// {
//     std::vector<Marker> Marks;
//     cv::Mat gray;
//     imageBinary(tempSetter,frame,gray);

//     //cv::imshow("twovalue",gray);
//     std::vector<std::vector<cv::Point>> contours;
//     std::vector<cv::Vec4i> Hierarchy;
//     cv::findContours(gray,contours,Hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);

//     for(int i=0;i<contours.size();i++)
//     {
//         if(contours[i].size()>minContourSize)
//         {
//             cv::RotatedRect rRect=cv::minAreaRect(contours[i]);
//             //length over wight
//             if(MIN(rRect.size.width,rRect.size.height)*supLengthWidth
//                > MAX(rRect.size.width,rRect.size.height))
//                 if(MIN(rRect.size.width,rRect.size.height)*infLengthWith
//                    <MAX(rRect.size.width,rRect.size.height))
//                 {
//                     Marker lb(rRect);
//                     if(lb.angle>50&&lb.angle<130)
//                     {
// //                        if(lb.area>8)//最小面积大于10
// //                        {
//                             Marks.push_back(lb);
//                             if(isShowGrayImage)
//                             {
//                                 lb.show(frame);
//                                 //std::cout<<lb.center<<std::endl;
//                             }
// //                        }
//                     }
//                 }
//         }
//     }
//     return Marks;
// }
std::vector<Marker> ArmourFinder::findMark(Setter &tempSetter,cv::Mat &frame)
{
    std::vector<Marker> Marks;
    cv::Mat gray;
    imageBinary(tempSetter,frame,gray);

    //cv::imshow("twovalue",gray);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> Hierarchy;
    cv::findContours(gray,contours,Hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);

    for(int i=0;i<contours.size();i++)
    {
        if(contours[i].size()>minContourSize)
        {
            cv::RotatedRect rRect=cv::minAreaRect(contours[i]);
            //length over wight
            if(MIN(rRect.size.width,rRect.size.height)*supLengthWidth
               > MAX(rRect.size.width,rRect.size.height))
                if(MIN(rRect.size.width,rRect.size.height)*infLengthWith
                   <MAX(rRect.size.width,rRect.size.height))
                {
                    Marker lb(rRect);
                    if(lb.angle>50&&lb.angle<130)
                    {
                       //if(lb.area>8)//最小面积大于10
                       //{
                       //std::cout << "lb.area" <<lb.area<<std::endl;
                            Marks.push_back(lb);
                            if(isShowGrayImage)
                            {
                                lb.show(frame);
                                //std::cout<<lb.center<<std::endl;
                            }
                       // }
                    }
                }
        }
    }
    return Marks;
}




// std::vector<Armour> ArmourFinder::findArmour(Setter &tempSetter,cv::Mat &frame,int &cmode)
// {
//     std::vector<Marker> Marks=findMark(tempSetter,frame);
//     std::cout<<Marks.size()<<std::endl;
//     std::vector<Armour> Armours;

//     for(int i=0;i<Marks.size();i++)
//         for(int j=i+1;j<Marks.size();j++)
//         {
//             if(abs(Marks[i].angle-Marks[j].angle)<12)//3
//                 if(abs(Marks[i].y-Marks[j].y)<mean(Marks[i].size,Marks[j].size)*0.8)//0.8
//                 if(p2pDistance(Marks[i].center,Marks[j].center)<mean(Marks[i].size,Marks[j].size)*3.5&&
//                     p2pDistance(Marks[i].center,Marks[j].center)>mean(Marks[i].size,Marks[j].size)*1.5)//3
//                     {     // std::cout<<(p2pDistance(Marks[i].center,Marks[j].center)/mean(Marks[i].size,Marks[j].size))<<std::endl;
//                         //float AB =p2pDistance(Marks[i].center,Marks[j].center)/mean(Marks[i].size,Marks[j].size);
//                         Armour armour(Marks[i],Marks[j]);
//                         cv::Mat testImage;
//                         armour.getIdImage(frame,testImage);
//                         cv::cvtColor(testImage,testImage,cv::COLOR_BGR2GRAY);
//                         int id=svm.predict(testImage);
//                         //std::cout<<id<<std::endl;
//                        // if(AB>=1.5&&AB<=)
//                         //if(id>=49) cmode=1;
//                         //else cmode=0;

//                         //if(id==0)continue;
//                         armour.getId(id);
//                         {
//                             Armours.push_back(armour);
//                                 armour.show(frame);
//                         }
//                     }
//  /*                else if(p2pDistance(Marks[i].center,Marks[j].center)<MAX(Marks[i].size,Marks[j].size)*3&&
//                            p2pDistance(Marks[i].center,Marks[j].center)>MAX(Marks[i].size,Marks[j].size)*1.5)
//                {
//                    Armour armour(Marks[i],Marks[j]);
//                    cv::Mat testImage;
//                    armour.getIdImage(frame,testImage);
//                    cv::cvtColor(testImage,testImage,cv::COLOR_BGR2GRAY);
//                    int id=svm.predict(testImage);
//                    //if(id==0)continue;
//                    armour.getId(id);
//                    {
//                        Armours.push_back(armour);
//                        armour.show(frame);
//                    }
//                }*/
//         }
//     //std::cout<<Armours.size()<<std::endl;
//     return Armours;
// }
//修改了这一段，目的是解决装甲板粘连问题
std::vector<Armour> ArmourFinder::findArmour(Setter &tempSetter, cv::Mat &frame, int &cmode)
{

    std::vector<Marker> Marks = findMark(tempSetter, frame);
    //std::cout << "size:" <<Marks.size() << std::endl;
    std::vector<Armour> Armours;
    
    // 计算所有标记的横坐标之和
    double sumX = 0.0;
    for (const Marker& mark : Marks) {
        sumX += mark.center.x;
    }
    // 计算横坐标的平均值
    double avgX = sumX / Marks.size();
    avg_X = avgX;
    // 选择两个横坐标离均值最近的标记作为装甲组合
    Marker closestMark1, closestMark2;
    double minDist = std::numeric_limits<double>::max(); // 初始设为最大值
    /*if(Marks.size()>=4)
    {

        for (int i = 0; i < Marks.size(); i++) 
        {
             for (int j = i + 1; j < Marks.size(); j++) 
            {
                if (abs(Marks[i].angle - Marks[j].angle) < 12
                && abs(Marks[i].y - Marks[j].y) < mean(Marks[i].size, Marks[j].size) * 0.8
                && p2pDistance(Marks[i].center, Marks[j].center) < mean(Marks[i].size, Marks[j].size) * 3.5
                && p2pDistance(Marks[i].center, Marks[j].center) > mean(Marks[i].size, Marks[j].size) * 1.5) 
           
                {
                //std::cout << "angle:" <<Marks[i].angle << std::endl;
                double dist1 = std::abs(Marks[i].center.x - avgX);
                double dist2 = std::abs(Marks[j].center.x - avgX);
                double totalDist = dist1 + dist2;

                    if (totalDist < minDist) 
                     {
                    minDist = totalDist;
                    closestMark1 = Marks[i];
                    closestMark2 = Marks[j];
                    }
                }
            }
        }
    }*/



        for (int i = 0; i < Marks.size(); i++) {
            for (int j = i + 1; j < Marks.size(); j++) {
                if (abs(Marks[i].angle - Marks[j].angle) < 12
                    && abs(Marks[i].y - Marks[j].y) < mean(Marks[i].size, Marks[j].size) * 0.8
                    && p2pDistance(Marks[i].center, Marks[j].center) < mean(Marks[i].size, Marks[j].size) * 4
                    && p2pDistance(Marks[i].center, Marks[j].center) > mean(Marks[i].size, Marks[j].size) * 1.5) 
                {
                    double dist1 = std::abs(Marks[i].center.x - avgX);
                    double dist2 = std::abs(Marks[j].center.x - avgX);
                    double totalDist = dist1 + dist2;

                    if (totalDist < minDist &&abs(Marks[i].y - Marks[j].y <=10)) {
                        minDist = totalDist;
                        closestMark1 = Marks[i];
                        closestMark2 = Marks[j];
                    }
                }
            }
        }
 
std::cout << "1:" <<closestMark1.y<< std::endl;
std::cout << "2:" <<closestMark2.y<< std::endl;
    if(Marks.size()>2){
    // 计算离avg_X最近的四块灯条的横坐标之和
    std::vector<std::pair<double, Marker>> distances; // 存储距离和对应的Marker
    for (const Marker& mark : Marks) {
        double distance = std::abs(mark.center.x - avgX);
        distances.emplace_back(distance, mark);
    }

    // 自定义比较函数
    auto compareMarkers = [](const std::pair<double, Marker>& a, const std::pair<double, Marker>& b) {
        return a.first < b.first; // 根据距离进行比较
    };

    // 按照距离排序
    std::sort(distances.begin(), distances.end(), compareMarkers);
    // 计算最近的四块灯条的横坐标均值
    double closestXSum = 0.0;
    int count = std::min(4, static_cast<int>(distances.size())); // 最多取四个
    for (int i = 0; i < count; i++) {
        closestXSum += distances[i].second.center.x;
    }
    center_X = closestXSum/4;

    fakecenter[0] = cv::Point2f(distances[2].second.center.x,distances[2].second.center.y);
    fakecenter[1] = cv::Point2f(distances[3].second.center.x,distances[3].second.center.y);
    // 计算除去最近两块灯条后另外两块的纵坐标均值
    float remainingYSum = 0.0;
    if (count >=4) {
        for (int i = 2; i <= 3; i++) {
            remainingYSum += distances[i].second.center.y;     
        }
         otheravg_Y = remainingYSum/2;
    }
    else if(count = 3) {otheravg_Y = distances[2].second.center.y;}

    // 计算除去最近两块灯条后另外两块的横坐标均值
    float remainingXSum = 0.0;
    if (count >=4) {
        for (int i = 2; i <= 3; i++) {
            remainingXSum += distances[i].second.center.x;
        }
         otheravg_X = remainingXSum/2;
    }
    else if(count = 3) {otheravg_X = distances[2].second.center.x;}
//     std::cout <<"中心坐标：" << center_X << std::endl;
// std::cout <<"两块横坐标" << otheravg_X << std::endl;
// std::cout <<"两块纵坐标" << otheravg_Y << std::endl;
}
otheravg_Y = 0;
otheravg_X = 0;

float AB =p2pDistance(closestMark1.center,closestMark2.center)/mean(closestMark1.size,closestMark2.size);
//std::cout <<"长宽比为：" << AB << std::endl;
float len = p2pDistance(closestMark1.center,closestMark2.center);
 angle_z =atan(((float)closestMark2.y - (float)closestMark1.y)/((float)closestMark2.x - (float)closestMark1.x)) * 180 / M_PI;
//std::cout <<"长度为" << len  << std::endl;
//std::cout <<"角度为" << angle_z  << std::endl;
//std::cout <<"closestMark1y:" << closestMark1.y  << "closestMark1x:"<< closestMark1.x <<std::endl;
//std::cout <<"closestMark2y:" << closestMark2.y  << "closestMark2x:"<< closestMark2.x <<std::endl;
    // 如果找到了最近的两个标记，则创建装甲对象并添加到装甲组合中
    if (minDist != std::numeric_limits<double>::max()) {
        Armour armour(closestMark1, closestMark2);
        cv::Mat testImage;
        armour.getIdImage(frame, testImage);
        cv::cvtColor(testImage, testImage, cv::COLOR_BGR2GRAY);
        int id = svm.predict(testImage);
        if(AB>=1.5&&AB<2.8)
         {cmode=0; 
          id=20;}
        if(AB>=2.8) 
          {cmode= 1;
           id = 50;}
        armour.getId(id);
        Armours.push_back(armour);
        armour.show(frame);
    }

    // 返回装甲组合向量
    return Armours;
}
void ArmourFinder::newBin(Setter &tempSetter,cv::Mat &src) {
    cv::Mat gray;
    std::vector<std::vector<cv::Point>> allContours;
    cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
    std::vector<Marker> markerGroup;
    gray = gray > 10;
    cv::findContours(gray.clone(), allContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    int conCount = int(allContours.size());
    for (int i = 0; i < conCount; i++) {
        cv::drawContours(gray, allContours, i, COLOR_W);//测试
        int cpSize = int(allContours[i].size());
        if (cpSize >= 20) {
            cv::RotatedRect rect = cv::minAreaRect(allContours[i]);
            float a, b, a_b;//a_b为长宽比
            cv::Point2f temp[4];
            rect.points(temp);
            a = p2pDistance(temp[0], temp[1]);
            b = p2pDistance(temp[1], temp[2]);
            if (a > b) {
                a_b = a / b;
            } else {
                a_b = b / a;
            }
            if (a_b >= 2 && a_b <= 15) {
                int blue = 0, red = 0;
                for (int j = 0; j < cpSize; j++) {
                    const uchar *inData = src.ptr<uchar>(allContours[i][j].y);
                    if (*(inData + allContours[i][j].x * 3 + 0) >
                        (*(inData + allContours[i][j].x * 3 + 2) + *(inData + allContours[i][j].x * 3 + 1))) {
                        blue++;
                    }
                    if (*(inData + allContours[i][j].x * 3 + 0) >
                        (*(inData + allContours[i][j].x * 3 + 1) + *(inData + allContours[i][j].x * 3 + 0))) {
                        red++;
                    }
                    char whoIam = 1;
                    if ((blue > cpSize * 0.5 && whoIam == RED) || (red > cpSize * 0.3 && whoIam == BLUE)) {
                        Marker Mtemp(rect);
                        markerGroup.push_back(Mtemp);
                        Mtemp.show(src);
                    }
                }
            }
        }
//        cv::imshow("gray", gray);
//        cv::waitKey(1);//这个之上为灯条寻找
    }
}

// std::vector<Armour>  ArmourFinder::getArmours(std::vector<Armour> &armour,float &angle_z,float &center_X,float &otheravg_Y)
// {
//     std::vector<Armour> filteredArmours[3];
    
// }

//根据当前装甲板坐标推出车中心的坐标和左右两块装甲板的坐标
// std::vector<cv::Point3f> ArmourFinder::getArmours(cv::Mat &tvec, float &angle_z)
// {
//     float x = std::round((tvec.at<double>(0) / 1000 + 0.071)/ 0.01) * 0.01;
//     float y = std::round(tvec.at<double>(1) / 1000 / 0.01) * 0.01;
//     float z = std::round(tvec.at<double>(2) / 1000 / 0.01) * 0.01;
//     float r = 0.2;
//     std::vector<cv::Point3f> threeArmours(3); // 定义一个包含3个元素的向量
//     float angle = angle_z *PI/180;

//         threeArmours[0] = cv::Point3f(x + r * sin(angle), y, z + r * cos(angle));//车中心坐标
//         threeArmours[1] = cv::Point3f(x + r * (sin(angle) + cos(angle)), y, z + r * (cos(angle) - sin(angle)));//右侧装甲板坐标
//         threeArmours[2] = cv::Point3f(x + r * (sin(angle) - cos(angle)), y, z + r * (sin(angle) + cos(angle)));//左侧装甲板坐标

//     return threeArmours; // 返回整个向量
// }





// std::vector<cv::Point2f> ArmourFinder::world2pixel(std::vector<cv::Point3f> objectPoints, CameraDistortion cal) {
//     cv::Mat_<double> tvec(3, 1); // 平移向量
//     cv::Mat_<double> rvec(3, 1); // 旋转向量
//     std::vector<cv::Point2f> imagePoint; // 存储投影后的像素坐标

//     // 使用projectPoints函数进行投影
//     cv::projectPoints(objectPoints, rvec, tvec, cal.CameraMdsdsdaatrix, cal.DistCoeffs, imagePoint);

//     return imagePoint; // 返回像素坐标
// }
std::vector<cv::Point3f> ArmourFinder::getArmours(cv::Mat &tvec, float &angle_z)
{
    float x = std::round((tvec.at<double>(0) / 1000 + 0.071) / 0.01) * 0.01;
    float y = std::round(tvec.at<double>(1) / 1000 / 0.01) * 0.01;
    float z = std::round(tvec.at<double>(2) / 1000 / 0.01) * 0.01;
    float r = 0.15;
    std::vector<cv::Point3f> threeArmours(4); // 定义一个包含3个元素的向量
    float angle = angle_z * PI / 180;

    threeArmours[0] = cv::Point3f(x , y, z ); // 当前装甲位置
    threeArmours[1] = cv::Point3f(x + r * (sin(angle) + cos(angle)), y, z + r * (cos(angle) - sin(angle))); // 右侧装甲板坐标
    threeArmours[2] = cv::Point3f(x + r * (sin(angle) - cos(angle)), y, z + r * (sin(angle) + cos(angle))); // 左侧装甲板坐标
    threeArmours[3] = cv::Point3f(x + r * sin(angle), y, z + r * cos(angle)); // 车中心坐标

   // std::cout << "current" <<threeArmours[0]<<std::endl;
     //std::cout << "right" <<threeArmours[1]<<std::endl;
     // std::cout << "left" <<threeArmours[2]<<std::endl;
     //  std::cout << "center" <<threeArmours[3]<<std::endl;
    // 选出最近的装甲板
    cv::Point3f closestArmour;
if(angle_z>=9) closestArmour =  threeArmours[1];
if(angle_z<=-9) closestArmour =  threeArmours[2];
else closestArmour = threeArmours[0];
    // 返回车中心、最近装甲板和当前装甲板到车中心的距离
    return {closestArmour};
}