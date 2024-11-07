#include "Svm.h"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include <dirent.h>
#include <cstring>

#define SVM cv::ml::SVM

Svm::Svm()
{
    svm=SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setDegree(0);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::Type::MAX_ITER,1000,0.01));
    svm->setGamma(1);
    svm->setCoef0(0);
    svm->setC(1);
    svm->setP(0);
}

void Svm::load(cv::String file_name)
{
    std::cout<<"I am loading the xml for svm,please wait.."<<std::endl;
    svm=cv::ml::StatModel::load<SVM>(file_name);
}

void Svm::pretreat(cv::Mat& testImage)
{
    if(testImage.channels()!=1)
        cv::cvtColor(testImage,testImage,cv::COLOR_BGR2GRAY);
    cv::threshold(testImage,testImage,0,1,cv::THRESH_OTSU);
    cv::resize(testImage,testImage,cv::Size(64,48));
    testImage=testImage.reshape(1,1);
    testImage.convertTo(testImage,CV_32FC1);
}

void Svm::GetFileNamesLabel(std::string path, std::vector<std::string> &filenames,std::vector<int>& labels)
{
    DIR *pDir;
    struct dirent*ptr;
    if(!(pDir=opendir(path.c_str())))
    {
        std::cout<<"Can't open the floder "<<path<<std::endl;
        return;
    }
    while((ptr=readdir(pDir))!=0)
    {
        if(strcmp(ptr->d_name,".")!=0&&strcmp(ptr->d_name,"..")!=0)
        {
            int label=0;
            for(int i=0;ptr->d_name[i]!=0;i++)
                label=label*10+ptr->d_name[i]-'0';
            std::cout<<path+"/"+ptr->d_name<<std::endl;
            std::cout<<label<<std::endl;

            filenames.push_back(path+"/"+ptr->d_name);
            labels.push_back(label);
        }
    }
    closedir(pDir);
}

int Svm::predict(cv::Mat testImage)
{
    pretreat(testImage);
    int result=svm->predict(testImage);
    return result;
}

void Svm::getData(cv::String file_name)
{
    std::vector<std::string> filenames;
    std::vector<int> labels;
    GetFileNamesLabel(file_name,filenames,labels);
    for(int i=0;i<filenames.size();i++)
    {
        std::vector<cv::String> imageName;
        cv::glob(filenames[i],imageName);
        for(int j=0;j<imageName.size();j++)
        {
            cv::Mat data=cv::imread(imageName[j]);
            //std::cout<<imageName[j]<<std::endl;
            cv::imshow("data",data);
            cv::waitKey(1);
            pretreat(data);
            trainingData.push_back(data);
            tariningLabels.push_back(labels[i]);
        }
    }
}

void Svm::train(cv::String dataPath,cv::String savePath)
{
    getData(dataPath);
    cv::Mat(tariningLabels).copyTo(classes);
    std::cout<<"start to train"<<std::endl;
    svm->train(trainingData,cv::ml::ROW_SAMPLE,classes);
    std::cout<<"saving ....."<<std::endl;
    svm->save(savePath);
    std::cout<<"finished"<<std::endl;
}