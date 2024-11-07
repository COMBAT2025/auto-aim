#include "../include/KalmanFilter.h"

Predict::Predict(PredictModel model,int key)
{
    if(1==key){
        stateNum = model.dynamParams;
        measureNum = model.measureParams;
        init(stateNum, measureNum);
        transitionMatrix = model.transMat;
        //std::cout<<model.transMat.type()<<std::endl;
        deltaTime=1/60.0;
        cv::setIdentity(measurementMatrix);
        cv::setIdentity(processNoiseCov, cv::Scalar::all(275));
        cv::setIdentity(measurementNoiseCov, cv::Scalar::all(25));
        cv::setIdentity(errorCovPost, cv::Scalar::all(1));
    }
    if(key==2){
        stateNum = 2;
        measureNum = 2;
        init(stateNum, measureNum);
        cv::setIdentity(transitionMatrix);
        cv::setIdentity(measurementMatrix);
        cv::setIdentity(processNoiseCov, cv::Scalar::all(5000.0f));
        cv::setIdentity(measurementNoiseCov, cv::Scalar::all(10000.0f));
        cv::setIdentity(errorCovPost);
    }

}

cv::Mat Predict::predict2(double deltaTime)
{
    predict();
    //µüŽúŽÎÊý
    int times = deltaTime / this->deltaTime;
    cv::Mat  state = statePost;
    for (int i = 0; i < times; i++)
    {
        //std::cout<<transitionMatrix<<"\n"<<state<<std::endl;
        state = transitionMatrix*state;
    }
    return state;
}
float Predict::smooth(float curValue)
{
    cv::Mat measurement=cv::Mat::zeros(2,1,CV_32F);
    measurement.at<float>(0)=curValue;
    measurement.at<float>(1)=0.0f;
    correct(measurement);
    cv::Mat prediction=predict();
    return prediction.at<float>(0);
}

cv::Mat & Predict::autoPredict(cv::Mat& measurement)
{
    correct(measurement);
    cv::Mat prediction=predict();
    return prediction;
}

void Predict::myInit(PredictModel model) {
    stateNum = model.dynamParams;
    measureNum = model.measureParams;
    init(stateNum, measureNum);
    transitionMatrix = model.transMat;
    cv::setIdentity(measurementMatrix);
    cv::setIdentity(processNoiseCov, cv::Scalar::all(275));
    cv::setIdentity(measurementNoiseCov, cv::Scalar::all(25));
    cv::setIdentity(errorCovPost, cv::Scalar::all(1));
}