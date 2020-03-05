//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <unistd.h>
#include "test.h"

int main()
{
	//读取图片（使用图片的绝对路径）
	//Mat src = imread("C://Users//php//Pictures//201802071152.png");
	//显示图片
	//imshow("Output", src);
	//显示灰度图
	//Mat Gray;
	//cvtColor(src, Gray, COLOR_BGR2GRAY);
	//imshow("Gray", Gray);

	//canny_();

	//GetContoursPic("C://Users//php//Desktop//7.png", "C://Users//php//Desktop//7-1.png");
	//myRecognition(imread("C://Users//php//Desktop//3.jpg"));
//	myVideoCapture();

    Test t;
//    t.cvtColor();
//    t.matRW();
    t.faceDetection();

	return 0;
}
