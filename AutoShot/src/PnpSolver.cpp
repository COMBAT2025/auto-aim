#include "PnpSolver.h"

PNPSolver::PNPSolver()
{

}
PNPSolver::PNPSolver(const Setter::ArmourSize as[3])
{
//    for(int i=0;i<3;i++)
//    {
//        spacePixel[0].push_back(cv::Point3f(-67,-28,0));
//        spacePixel[0].push_back(cv::Point3f(-67,28,0));
//        spacePixel[0].push_back(cv::Point3f(67,28,0));
//        spacePixel[0].push_back(cv::Point3f(67,-28,0));
    spacePixel[0].push_back(cv::Point3f(0,0,0));
    spacePixel[0].push_back(cv::Point3f(0,56,0));
    spacePixel[0].push_back(cv::Point3f(134,56,0));
    spacePixel[0].push_back(cv::Point3f(134,0,0));
    spacePixel[1].push_back(cv::Point3f(0,0,0));
    spacePixel[1].push_back(cv::Point3f(0,56,0));
    spacePixel[1].push_back(cv::Point3f(230,56,0));
    spacePixel[1].push_back(cv::Point3f(230,0,0));
    spacePixel[2].push_back(cv::Point3f(0,0,0));
    spacePixel[2].push_back(cv::Point3f(0,56,0));
    spacePixel[2].push_back(cv::Point3f(230,56,0));
    spacePixel[2].push_back(cv::Point3f(230,0,0));
    //}
}

cv::Mat PNPSolver::armourDistance(Point2f_v points,char mode,CameraDistortion cal,cv::Mat &rvec)
//mode是大装甲还是小装甲
{
    if(points.empty()){return cv::Mat();}
   // cv::Mat_<double> rvec(3,1);   //旋转矩阵
    cv::Mat_<double> tvec(3,1);   //平移矩阵
    //cv::solvePnP(spacePixel[mode],points,cal.CameraMatrix,cal.DistCoeffs,rvec,tvec,false,cv::SOLVEPNP_AP3P);
    cv::solvePnP(spacePixel[mode],points,cal.CameraMatrix,cal.DistCoeffs,rvec,tvec,false,cv::SOLVEPNP_ITERATIVE);
//    printf("第一个%f\n",float(rvec.at<double>(0)*180.0/PI));
//    printf("第二个%f\n",float(rvec.at<double>(1)*180.0/PI));
//    printf("第三个%f\n",float(rvec.at<double>(2)*180.0/PI));
//    char a;
//    a=cv::waitKey(1);
//    if(a=='q'){
//        cv::waitKey(0);
//    }
    return tvec;
}

cv::Mat PNPSolver::bigBufferDistance(Point2f_v points,CameraDistortion cal)
{
    cv::Mat_<double> rvec(3,1);//旋转矩阵
    cv::Mat_<double> tvec(3,1);//平移矩阵
    cv::solvePnP(spacePixel[BUFFER],points,cal.CameraMatrix,cal.DistCoeffs,rvec,tvec,false,cv::SOLVEPNP_AP3P);
    return tvec;
}