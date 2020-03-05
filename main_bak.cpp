#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;

Mat src_gray;
int thresh = 150;
RNG rng(12345);
void thresh_callback(int, void*);

void canny_() {
	Mat srcImg = imread("C://Users//php//Desktop//20190514091030.png");
	Mat edge;
	Canny(srcImg, edge, 100, 300);
	imshow("edge", edge);
}

//第一个参数：输入图片名称；第二个参数：输出图片名称
void GetContoursPic(const char* pSrcFileName, const char* pDstFileName)
{
	Mat srcImg = imread(pSrcFileName);
	imshow("原始图", srcImg);
	Mat gray, binImg;
	//灰度化
	cvtColor(srcImg, gray, COLOR_RGB2GRAY);
	imshow("灰度图", gray);
	//二值化
	threshold(gray, binImg, 100, 200, THRESH_BINARY);
	imshow("二值化", binImg);

	vector<vector<Point> > contours;
	vector<Rect> boundRect(contours.size());
	//注意第5个参数为CV_RETR_EXTERNAL，只检索外框
	findContours(binImg, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //找轮廓
	cout << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		//需要获取的坐标
		Point2f rectpoint[4];
		auto rect = minAreaRect(Mat(contours[i]));
		rect.points(rectpoint);
		// boxPoints(rect, rectpoint); //获取4个顶点坐标
									  //与水平线的角度
		float angle = rect.angle;
		cout << angle << endl;

		int line1 = sqrt((rectpoint[1].y - rectpoint[0].y)*(rectpoint[1].y - rectpoint[0].y) + (rectpoint[1].x - rectpoint[0].x)*(rectpoint[1].x - rectpoint[0].x));
		int line2 = sqrt((rectpoint[3].y - rectpoint[0].y)*(rectpoint[3].y - rectpoint[0].y) + (rectpoint[3].x - rectpoint[0].x)*(rectpoint[3].x - rectpoint[0].x));
		//rectangle(binImg, rectpoint[0], rectpoint[3], Scalar(255), 2);
		//面积太小的直接pass
		if (line1 * line2 < 600)
		{
			continue;
		}

		//为了让正方形横着放，所以旋转角度是不一样的。竖放的，给他加90度，翻过来
		if (line1 > line2)
		{
			angle = 90 + angle;
		}

		//新建一个感兴趣的区域图，大小跟原图一样大
		Mat RoiSrcImg(srcImg.rows, srcImg.cols, CV_8UC3); //注意这里必须选CV_8UC3
		RoiSrcImg.setTo(0); //颜色都设置为黑色
							//imshow("新建的ROI", RoiSrcImg);
							//对得到的轮廓填充一下
		drawContours(binImg, contours, -1, Scalar(255), FILLED);

		//抠图到RoiSrcImg
		srcImg.copyTo(RoiSrcImg, binImg);


		//再显示一下看看，除了感兴趣的区域，其他部分都是黑色的了
		namedWindow("RoiSrcImg", 1);
		imshow("RoiSrcImg", RoiSrcImg);

		//创建一个旋转后的图像
		Mat RatationedImg(RoiSrcImg.rows, RoiSrcImg.cols, CV_8UC1);
		RatationedImg.setTo(0);
		//对RoiSrcImg进行旋转
		Point2f center = rect.center;  //中心点
		Mat M2 = getRotationMatrix2D(center, angle, 1);//计算旋转加缩放的变换矩阵
		warpAffine(RoiSrcImg, RatationedImg, M2, RoiSrcImg.size(), 1, 0, Scalar(0));//仿射变换
		imshow("旋转之后", RatationedImg);
		imwrite("r.jpg", RatationedImg); //将矫正后的图片保存下来
	}

#if 1
	//对ROI区域进行抠图

	//对旋转后的图片进行轮廓提取
	vector<vector<Point> > contours2;
	Mat raw = imread("r.jpg");
	Mat SecondFindImg;
	//SecondFindImg.setTo(0);
	cvtColor(raw, SecondFindImg, COLOR_BGR2GRAY);  //灰度化
	threshold(SecondFindImg, SecondFindImg, 80, 200, THRESH_BINARY);
	findContours(SecondFindImg, contours2, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	//cout << "sec contour:" << contours2.size() << endl;

	for (int j = 0; j < contours2.size(); j++)
	{
		//这时候其实就是一个长方形了，所以获取rect
		Rect rect = boundingRect(Mat(contours2[j]));
		//面积太小的轮廓直接pass,通过设置过滤面积大小，可以保证只拿到外框
		if (rect.area() < 600)
		{
			continue;
		}
		Mat dstImg = raw(rect);
		imshow("dst", dstImg);
		imwrite(pDstFileName, dstImg);
	}
#endif
}

int myRecognition(Mat src) {
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		return -1;
	}
	const char* source_window = "Source";
	//namedWindow(source_window);
	imshow(source_window, src);
	//灰度化
	cvtColor(src, src_gray, COLOR_RGB2GRAY); // imshow("灰度图", src_gray);

	const int max_thresh = 255;
	createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);
}

void myVideoCapture() {
	VideoCapture capture(0);//打开摄像头
	if (!capture.isOpened())//没有打开摄像头的话，就返回。
		return ;
	Mat edges; //定义一个Mat变量，用于存储每一帧的图像
	Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	while (1)
	{
        auto t_old = std::chrono::high_resolution_clock::now();
		Mat frame; //定义一个Mat变量，用于存储每一帧的图像
		capture >> frame;  //读取当前帧
		if (frame.empty())
		{
			break;
		}
		else
		{
			cvtColor(frame, edges, COLOR_BGR2GRAY);//彩色转换成灰度

			threshold(edges, edges, 100, 250, THRESH_BINARY_INV);  //imshow("二值化", edges);
			//blur(edges, edges, Size(7, 7));//模糊化
			//Canny(edges, edges, 0, 30, 3);//边缘化
			vector<vector<Point> > contours;
			findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //找轮廓

			for (size_t i = 0; i< contours.size(); i++)
			{
				Point2f rect_points[4];
				RotatedRect minRect = minAreaRect(contours[i]);
				minRect.points(rect_points);
				int line1 = sqrt((rect_points[1].y - rect_points[0].y)*(rect_points[1].y - rect_points[0].y) + (rect_points[1].x - rect_points[0].x)*(rect_points[1].x - rect_points[0].x));
				int line2 = sqrt((rect_points[3].y - rect_points[0].y)*(rect_points[3].y - rect_points[0].y) + (rect_points[3].x - rect_points[0].x)*(rect_points[3].x - rect_points[0].x));
				//面积太小的直接pass
				if (line1 * line2 < 600)
				{
					continue;
				}
				/*cout << "points: [" << rect_points[0].x << ", " << rect_points[0].y << "] ["
					<< rect_points[1].x << ", " << rect_points[1].y << "] ["
					<< rect_points[2].x << ", " << rect_points[2].y << "] ["
					<< rect_points[3].x << ", " << rect_points[3].y << "] [" << endl;*/
//				cout << "center: " << minRect.center << "    angle: " << minRect.angle << "  |  " << line1 << ", " << line2<< endl;

				for (int j = 0; j < 4; j++)
				{
					line(frame, rect_points[j], rect_points[(j + 1) % 4], Scalar(0,0,255), 2);
				}
			}
//            imwrite("r.jpg", frame); //将矫正后的图片保存下来
//			imshow("识别结果", frame); //显示当前帧
		}
        cout <<"   t: "<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t_old).count()<< endl;
//		waitKey(20); //延时30ms
        usleep(1000 * 200);
	}
	capture.release();//释放资源
	destroyAllWindows();//关闭所有窗口
}



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

	waitKey(0);

	return 0;
}

void thresh_callback(int, void*)
{

	vector<vector<Point> > contours;
	threshold(src_gray, src_gray, thresh, 200, THRESH_BINARY); imshow("二值化", src_gray);
	//blur(src_gray, src_gray, Size(2, 2)); imshow("滤波", src_gray);

	findContours(src_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //找轮廓
	cout << contours.size() << endl;
	Mat drawing = Mat::zeros(src_gray.size(), CV_8UC3);
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		//drawContours(drawing, contours, (int)i, color); // 绘制轮廓
		Point2f rect_points[4];
		RotatedRect minRect = minAreaRect(contours[i]);
		minRect.points(rect_points);

		int line1 = sqrt((rect_points[1].y - rect_points[0].y)*(rect_points[1].y - rect_points[0].y) + (rect_points[1].x - rect_points[0].x)*(rect_points[1].x - rect_points[0].x));
		int line2 = sqrt((rect_points[3].y - rect_points[0].y)*(rect_points[3].y - rect_points[0].y) + (rect_points[3].x - rect_points[0].x)*(rect_points[3].x - rect_points[0].x));
		//rectangle(binImg, rectpoint[0], rectpoint[3], Scalar(255), 2);
		//面积太小的直接pass
		if (line1 * line2 < 600)
		{
			continue;
		}
		cout << "points: [" << rect_points[0].x << ", " << rect_points[0].y << "] ["
			<< rect_points[1].x << ", " << rect_points[1].y << "] ["
			<< rect_points[2].x << ", " << rect_points[2].y << "] ["
			<< rect_points[3].x << ", " << rect_points[3].y << "] [" << endl;
		cout<<"center: " << minRect.center << "    angle: " << minRect.angle << "  |  " << line1 << ", " << line2 << endl;

		for (int j = 0; j < 4; j++)
		{
			line(drawing, rect_points[j], rect_points[(j + 1) % 4], color);
		}
	}
	imshow("Contours", drawing);
}