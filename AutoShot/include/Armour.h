#ifndef ARMOUR_H
#define ARMOUR_H


#include "Combat2024_AllTeam.h"
#include "Marker.h"

class Armour {

public:
    Armour();
    //input two mark,whatever who is Left;
    Armour(Marker& markLeft,Marker& markRight);
    void show(cv::Mat &frame);
    void getIdImage(cv::Mat &frame,cv::Mat &boundingImage);
    void getId(int No);
    int my_height;
    int id;
    bool isSudden;
    char name[5];     //0字符ID，1大小（L,S）,2结束符，3模式（0小1大），4 数字ID
    std::vector<cv::Point2f> concors;
    cv::Point2f center;
    float angle_z;
    /*
     *   ///////////////////////////////////
     *   //                               //
     *   //  1                            //   4
     * // //                             // //
     * // //                             // //
     * // //                             // //
     * // //                             // //
     * // // 2                           // //  3
     *   //                               //
     *   //                               //
     *   ///////////////////////////////////
     */

};

#endif //Combat2024_ARMOUR_H
