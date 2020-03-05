//
// Created by php on 19-10-16.
//

#include "test.h"
using namespace cv;
void Test::cvtColor()
{
//    读取图片（使用图片的绝对路径）
    cv::Mat src = cv::imread("../imgs/logo.jpg"); // cv::imread("../imgs/logo.jpg", IMREAD_GRAYSCALE );
    cv::imshow("input", src);
    cv::Mat tmp;
    cv::cvtColor(src, tmp, cv::COLOR_BGR2GRAY); // 转为灰度图
    cv::imshow("GRAY", tmp);

//    # RGB to HSV
    cv::cvtColor(src, tmp, COLOR_BGR2HSV);
    cv::imshow("HSV", tmp);

    cv::cvtColor(src, tmp, COLOR_BGR2YUV);
    cv::imshow("COLOR_BGR2YUV", tmp);

    cv::cvtColor(src, tmp, COLOR_BGR2YCrCb);
    cv::imshow("COLOR_BGR2YCrCb", tmp);

    flip(src, tmp, 1);cv::imshow("flip", tmp);// 0垂直翻转 1   -1
    cv::waitKey(0);

}

/**
 * https://github.com/JimmyHHua/opencv_tutorials/blob/master/python/code_004/opencv_004.py
 *
*/
void Test::matRW() {
    cv::Mat src = cv::imread("../imgs/logo.jpg");
    cv::imshow("input", src);
    std::cout<<"width: "<<src.cols<<", height:"<<src.rows<<std::endl;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            auto bgr = src.at<Vec3b>(i, j);
//            Vec3b bgr2 = {255-bgr[0], 255-bgr[0]}, 255-bgr[0]}};
            src.at<Vec3b>(i, j)[0] = 255-bgr[0];
            src.at<Vec3b>(i, j)[1] = 255-bgr[1];
            src.at<Vec3b>(i, j)[2] = 255-bgr[2];
        }
    }
    cv::imshow("output", src);
    cv::waitKey(0);
}


/**
 * https://blog.csdn.net/u012679707/article/details/80376969
 * */
void Test::faceDetection() {
    Mat img = imread("../imgs/she.jpg");
    namedWindow("display");
    imshow("display", img);

    /*********************************** 1.加载人脸检测器  ******************************/
    // 建立级联分类器
    CascadeClassifier cascade;
    // 加载训练好的 人脸检测器（.xml）
    const std::string path = "../data/haarcascades/haarcascade_frontalface_alt.xml";
    if (!cascade.load(path))
    {
        std::cout << "cascade load failed!\n";
    }

    //计时
    double t = 0;
    t = (double)getTickCount();
    /*********************************** 2.人脸检测 ******************************/
    std::vector<Rect> faces(0);
    cascade.detectMultiScale(img, faces, 1.1, 2, 0 ,Size(30,30));

    std::cout << "detect face number is :" << faces.size() << std::endl;
    /********************************  3.显示人脸矩形框 ******************************/

    if (faces.size() > 0)
    {
        for (size_t i = 0;i < faces.size();i++)
        {
            rectangle(img, faces[i], Scalar(0, 0, 250), 1, 8, 0);
        }
    }
    else std::cout << "未检测到人脸" << std::endl;

    t = (double)getTickCount() - t;  //getTickCount():  Returns the number of ticks per second.
    std::cout << "检测人脸用时：" << t * 1000 / getTickFrequency() << "ms (不计算加载模型的时间）" << std::endl;

    namedWindow("face_detect");
    imshow("face_detect", img);
    waitKey(0);
}