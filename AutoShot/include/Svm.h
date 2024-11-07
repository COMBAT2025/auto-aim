#ifndef SVM_H
#define SVM_H

#include "Combat2024_AllTeam.h"
#include<opencv2/opencv.hpp>

class Svm {
public:
    //build a svm for training
    Svm();
    void load(cv::String file_name);
    //collecting data for training
    void getData(cv::String file_name);

    void train(cv::String dataPath,cv::String savePath);
    int predict(cv::Mat testImage);

private:
    cv::Ptr<cv::ml::SVM> svm;

    //data for training,include image,label
    cv::Mat trainingData,classes;
    std::vector<int> tariningLabels;

    //pretraetment of testImage for svm(training or predict)
    void pretreat(cv::Mat& testImage);
    void GetFileNamesLabel(std::string path,
                           std::vector<std::string>&filenames,std::vector<int>& labels);
};


#endif //SVM_H