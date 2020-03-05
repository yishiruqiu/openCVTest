//
// Created by php on 19-10-16.
// 学习 https://github.com/JimmyHHua/opencv_tutorials/blob/master/README_CN.md
// 官方文档 https://docs.opencv.org/4.1.1/

#ifndef OPENCVTEST_TEST_H
#define OPENCVTEST_TEST_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

class Test {
public:
//    Test();
    void  cvtColor();   //色彩转换 灰度图
    void  matRW();      //读写
    void faceDetection();   //人脸检测
private:

};


#endif //OPENCVTEST_TEST_H
