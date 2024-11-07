#include "EasyFind.h"

EasyFind::EasyFind() {
    svm.load("../tools/parameters_lists/SVM.xml");
}

void EasyFind::findme(cv::Mat &src, SerialPort &tempSerial,char whoIAm,std::vector<Armour> &Armors) {
    cv::cvtColor(src,gray,cv::COLOR_RGB2GRAY);
    gray=gray>100;
    cv::findContours(gray.clone(),allContours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);
    int conCount=int(allContours.size());
    for(int i=0;i<conCount;i++)
    {
        cv::drawContours(gray,allContours,i,COLOR_W);//测试
        int cpSize=int(allContours[i].size());
        if(cpSize>=20){
            cv::RotatedRect rect = cv::minAreaRect(allContours[i]);
            float a,b,a_b;//a_b为长宽比
            cv::Point2f temp[4];
            rect.points(temp);
            a=p2pDistance(temp[0],temp[1]);
            b=p2pDistance(temp[1],temp[2]);
            if(a>b){
                a_b=a/b;
            }else{
                a_b=b/a;
            }
            if(a_b>=2&&a_b<=15){
                int blue=0,red=0;
                for(int j=0;j<cpSize;j++){
                    const uchar* inData=src.ptr<uchar>(allContours[i][j].y);
                    if(*(inData+allContours[i][j].x*3+0)>(*(inData+allContours[i][j].x*3+2)+*(inData+allContours[i][j].x*3+1))){
                        blue++;
                    }
                    if(*(inData+allContours[i][j].x*3+0)>(*(inData+allContours[i][j].x*3+1)+*(inData+allContours[i][j].x*3+0))){
                        red++;
                    }
                    if((blue>cpSize*0.5&&whoIAm==RED)||(red>cpSize*0.3&&whoIAm==BLUE)){
                        Marker Mtemp(rect);
                        markerGroup.push_back(Mtemp);
                        Mtemp.show(src);
                    }
                }
            }
        }
        cv::imshow("gray",gray);
        cv::waitKey(1);//这个之上为灯条寻找
        int markerSize=int(markerGroup.size());
        for(int i=0;i<markerSize;i++){
            for(int j=i+1;j<markerSize;j++){
                float mHeight=(p2pDistance(markerGroup[i].LedUp,markerGroup[i].LedDown)
                        +p2pDistance(markerGroup[i].LedUp,markerGroup[i].LedDown))/2;//灯条高度
                if(p2pDistance(markerGroup[i].center,markerGroup[j].center)/mHeight<=6&&
                        p2pDistance(markerGroup[i].center,markerGroup[j].center)/mHeight>=1){
                    cv::Point2f points[4];
                    cv::Point2f pointss[4];
                    adjustOrder(markerGroup[i],markerGroup[j],points);
                    pointss[0]=cv::Point(0,0);
                    pointss[1]=points[1]-points[0];
                    pointss[2]=points[2]-points[0];
                    pointss[3]=points[3]-points[0];
                    float PImage[3][3] = {(pointss[2].x-pointss[3].x)/129, (pointss[2].x-pointss[1].x)/56,pointss[1].x-pointss[2].x+pointss[3].x,
                                          (pointss[2].y-pointss[3].y)/129, (pointss[2].y-pointss[1].y)/56,pointss[1].y-pointss[2].y+pointss[3].y,
                                          0,0,1};
                    cv::Point2f stdLength = pointss[1]+pointss[3];
                    float temp = sqrtf(stdLength.x*stdLength.x+stdLength.y*stdLength.y);

                    cv::Mat res =  cv::Mat(3,3,CV_32FC1,PImage);
                    float tx = res.at<float>(0,2);
                    float ty = res.at<float>(1,2);
                    float w = sqrtf((pointss[2]-pointss[3]).x*(pointss[2]-pointss[3]).x+(pointss[2]-pointss[3]).y*(pointss[2]-pointss[3]).y);
                    float h = sqrtf((pointss[2]-pointss[3]).x*(pointss[2]-pointss[3]).x+(pointss[2]-pointss[3]).y*(pointss[2]-pointss[3]).y);
                    if(w/temp>0.1 && w/temp<1 && h/temp>0.1 && h/temp<1){
                        if(sqrtf(tx*tx+ty*ty)/temp < 0.2){
                            if(markerGroup[i].center.x<markerGroup[j].center.x){
                                Armour armour(markerGroup[i],markerGroup[j]);
                                Armors.push_back(armour);
                                armour.show(src);
                            }
                        }
                    }
                }
            }
        }
    }
}

void EasyFind::adjustOrder(Marker m1,Marker m2,cv::Point2f points[])
{//这个排序得按szw的来
    if(m1.center.x<m2.center.x){
        if(m1.LedUp.y<=m1.LedDown.y){
            points[0]=m1.LedUp;
            points[3]=m1.LedDown;
        }else{
            points[3]=m1.LedUp;
            points[0]=m1.LedDown;
        }
        if(m2.LedUp.y<=m2.LedDown.y){
            points[1]=m2.LedUp;
            points[2]=m2.LedDown;
        }else{
            points[2]=m2.LedUp;
            points[1]=m2.LedDown;
        }
    }else{
        if(m1.LedUp.y<=m1.LedDown.y){
            points[1]=m1.LedUp;
            points[2]=m1.LedDown;
        }else{
            points[2]=m1.LedUp;
            points[1]=m1.LedDown;
        }
        if(m2.LedUp.y<=m2.LedDown.y){
            points[0]=m2.LedUp;
            points[3]=m2.LedDown;
        }else{
            points[3]=m2.LedUp;
            points[0]=m2.LedDown;
        }
    }
}