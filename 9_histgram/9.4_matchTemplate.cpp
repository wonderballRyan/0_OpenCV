#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

//-------------------------【宏定义部分】--------------------------
//描述：定义一些辅助宏
//-----------------------------------------------------------------
#define WINDOW_NAME1 "【原始图片】"  //为窗口标题定义的宏
#define WINDOW_NAME2 "【效果窗口】"  //为窗口标题定义的宏

//--------------------------【全局变量声明部分】---------------------------
//描述：全局变量的声明
//-------------------------------------------------------------------------
Mat g_srcImage;
Mat g_templateImage;
Mat g_resultImage;
int g_nMatchMethod;
int g_nMaxTrackbarNum = 5;

//------------------------【全局函数声明部分】------------------------
//描述：全局函数的声明
//-------------------------------------------------------------------
void on_Matching(int, void*);

int main()
{
	//【1】载入源图像和模板块
	g_srcImage = imread("F:\\opencv\\images\\tomato_3.jpg", 1);
	g_templateImage = imread("F:\\opencv\\images\\tomato_4.jpg", 1);

	//【2】创建窗口
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);

	//【3】创建滑动条并进行一次初始化
	createTrackbar("方法", WINDOW_NAME1, &g_nMatchMethod, g_nMaxTrackbarNum, on_Matching);
	on_Matching(0, 0);

	waitKey(0);
	return 0;
}

//----------------------------【on_Matching()函数】--------------------------------------
//描述：回调函数
//---------------------------------------------------------------------------------------
void on_Matching(int, void*)
{
	//【1】给局部变量初始化
	Mat srcImage;
	g_srcImage.copyTo(srcImage);

	//【2】初始化用于结果输出的矩阵
	int resultImage_cols = g_srcImage.cols - g_templateImage.cols + 1;
	int resultImage_rows = g_srcImage.rows - g_templateImage.rows + 1;
	g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	//【3】进行匹配和标准化
	matchTemplate(g_srcImage, g_templateImage, g_resultImage, g_nMatchMethod);
	normalize(g_resultImage, g_resultImage, 0, 1, NORM_MINMAX, -1, Mat());

	//【4】通过函数minMaxLoc定位最匹配的位置
	double minValue;
	double maxValue;
	Point minLocation;
	Point maxLocation;
	Point matchLocation;
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat());

	//【5】对于方法SQDIFF和SQDIFF_NORMED，越小的数值有着更高的匹配结果，而其余的方法，数值大匹配效果越好
	if (g_nMatchMethod == TM_SQDIFF || g_nMatchMethod == TM_CCOEFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	//【6】绘制出矩形，并显示最终结果
	rectangle(srcImage, matchLocation, Point(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);
	rectangle(g_resultImage, matchLocation, Point(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow(WINDOW_NAME1, srcImage);
	imshow(WINDOW_NAME2, g_resultImage);
}
