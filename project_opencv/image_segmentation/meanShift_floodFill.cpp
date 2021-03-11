#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main(int argc, char** argv)
{
	Mat img = imread("F:\\opencv\\prj_imgSegmentation\\images\\2.jpg"); //读入图像，RGB三通道  
	imshow("原图像", img);
	Mat res; //分割后图像
	int spatialRad = 50;  //空间窗口大小
	int colorRad = 50;   //色彩窗口大小
	int maxPyrLevel = 2;  //金字塔层数
	pyrMeanShiftFiltering(img, res, spatialRad, colorRad, maxPyrLevel); //色彩聚类平滑滤波
	imshow("res", res);
	RNG rng = theRNG();
	Mat mask(res.rows + 2, res.cols + 2, CV_8UC1, Scalar::all(0));  //掩模
	for (int y = 0; y < res.rows; y++)
	{
		for (int x = 0; x < res.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)  //非0处即为1，表示已经经过填充，不再处理
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(res, mask, Point(x, y), newVal, 0, Scalar::all(5), Scalar::all(5)); //执行漫水填充
			}
		}
	}
	imshow("meanShift图像分割", res);
	waitKey();
	return 0;
}
