// Filtering.cpp
#include "Filtering.h"

float Filtering::loop(float Z) {
    float measured_distance = Z; // 测量值
    float distance = KLM(measured_distance);
    return distance;
}

float Filtering::KLM(float Z) {
    float P = 1;
    float P_; // 对应公式中的p'
    float X = 0;
    float X_; // X'
    float K = 0;
    float Q = 0.01; // 噪声
    float R = 0.2;  // R如果很大，更相信预测值，那么传感器反应就会迟钝，反之相反
    X_ = X + 0;
    P_ = P + Q;
    K = P_ / (P_ + R);
    X = X_ + K * (Z - X_);
    P = P_ - K * P_;
    return X;
}