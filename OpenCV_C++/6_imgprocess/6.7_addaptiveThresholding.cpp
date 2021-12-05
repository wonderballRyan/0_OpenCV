#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;


//-----------------------------------【全局函数声明】--------------------------------------
//描述：全局函数声明
//-----------------------------------------------------------------------------------------
Mat srcImage, grayImage,dstImage;
int g_nMeanBlurValue = 3;//均值滤波参数值

//-----------------------------------【全局函数声明】--------------------------------------
//描述：全局函数声明
//-----------------------------------------------------------------------------------------
static void on_MeanBlur(int, void*);

int main()
{
    // read an image
    Mat srcImage = imread("F:\\img_process\\imgs\\4.jpg");
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

    //blur
    on_MeanBlur(g_nMeanBlurValue,0);

    // adaptive
    Mat dst1, dst2, dst3;
    threshold(dstImage, dst1, 100, 255, THRESH_BINARY);
    adaptiveThreshold(dstImage, dst2, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);
    adaptiveThreshold(dstImage, dst3, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);

    // show images
    imshow("img", grayImage);
    imshow("threshold", dst1);
    imshow("mean_c", dst2);
    imshow("gauss_c", dst3);
    //imwrite("F:\\img_process\\imgs\\20210127\\1.adaptive.png", dst2);
    waitKey(0);
}

static void on_MeanBlur(int, void*)
{
    //均值滤波操作
    blur(grayImage, dstImage, Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1));

}
