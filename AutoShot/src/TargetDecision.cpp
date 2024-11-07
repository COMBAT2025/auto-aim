#include "TargetDecision.h"
TargetDecision::TargetDecision() {

}
TargetDecision::TargetDecision(std::vector<Armour> enemyArmours) {
    if(!enemyArmours.empty()){
        int count=enemyArmours.size();
        int min=500;
        for(int i=0;i<count;i++){
            TargetDecision::armorDistance.emplace_back(p2pDistance(enemyArmours[i].center,cv::Point(400,300)));
            //std::cout<<"第"<<i<<"个"<<p2pDistance(enemyArmours[i].center,cv::Point(400,300))<<std::endl;
        }//计算所有装甲中心距离摄像头中心的距离
        for(int i=0;i<count;i++){
            if(armorDistance[i]<min)
            {
                min=armorDistance[i];
                TargetDecision::target=i;
            /*if(1==enemyArmours[i].id)
                armorPoints[i]=10;
            else
                armorPoints[i]=5;
            if(enemyArmours[i].my_height<65)
                armorPoints[i] *=5;
            else
                armorPoints[i] *=5;*/
            }
        /*int max=0;
        for(int i=0;i<count;i++){
            if(armorPoints[i]>max){
                target=i;
                max=enemyArmours[i].my_height;
            }*/
        }
        //std::cout<<enemyArmours[target].center<<std::endl;
    }
    else{
        TargetDecision::target=-1;
    }
    //if(target!=-1) std::cout<<"目标是第"<<target<<"个装甲"<<std::endl;
}

int TargetDecision::PrintTarget() {
    return target;
}
