#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp> 
#include <string>
using namespace cv;
using namespace std;

void SimplestCB(Mat& in, Mat& out, float percent) {
    assert(in.channels() == 3);
    assert(percent > 0 && percent < 100);

    float half_percent = percent / 200.0f;

    vector<Mat> tmpsplit; split(in, tmpsplit);
    for (int i = 0; i<3; i++) {
        Mat flat; tmpsplit[i].reshape(1, 1).copyTo(flat);
        sort(flat, flat, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
        int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
        int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
        cout << lowval << " " << highval << endl;        
        tmpsplit[i].setTo(lowval, tmpsplit[i] < lowval);
        tmpsplit[i].setTo(highval, tmpsplit[i] > highval);
        normalize(tmpsplit[i], tmpsplit[i], 0, 255, NORM_MINMAX);
    }
    merge(tmpsplit, out);
}



int main(int argc, char** argv)
{
    // метод простого цветового баланса
    Mat tmp, im = imread("1.jpg");
    SimplestCB(im, tmp, 1);
    imwrite("1balanced.jpg", tmp);


    //метод гистограммы
    Mat bgr_image = imread("1.jpg");
    Mat lab_image;
    cvtColor(bgr_image, lab_image, CV_BGR2Lab);
    vector<Mat> lab_planes(3);
    split(lab_image, lab_planes);  
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(4);
    Mat dst;
    clahe->apply(lab_planes[0], dst);
    dst.copyTo(lab_planes[0]);
    merge(lab_planes, lab_image);
    Mat image_clahe;
    cvtColor(lab_image, image_clahe, CV_Lab2BGR);
    imwrite("1CLAHE.jpg", image_clahe);

    // метод серого мира
    Mat imgIn = imread("1.jpg");
    Scalar sumImg = sum(imgIn);
    Scalar illum = sumImg / (imgIn.rows*imgIn.cols);
    vector<Mat> rgbChannels(3);
    split(imgIn, rgbChannels);
    Mat redImg = rgbChannels[2];
    Mat graanImg = rgbChannels[1];
    Mat blueImg = rgbChannels[0];
    double scale = (illum(0) + illum(1) + illum(2)) / 3;
    redImg = redImg*scale / illum(2);
    graanImg = graanImg*scale / illum(1);
    blueImg = blueImg*scale / illum(0);
    rgbChannels[0] = blueImg;
    rgbChannels[1] = graanImg;
    rgbChannels[2] = redImg;
    Mat imgOut;
    merge(rgbChannels, imgOut);
    imwrite("1GreyWorld.jpg", imgOut);

    waitKey(0);
    return 0;

}