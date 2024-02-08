#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
// #include "emscripten.h"
#include <vector>
#include <bitset>
#include <list>
#include <algorithm>

using namespace std;
using namespace cv;

typedef int color_t;
typedef unsigned int level_t;

class RGB{
    public:
        color_t b;
        color_t g;
        color_t r;

        RGB(color_t R,color_t G,color_t B){
            b = B;
            g = G;
            r = R;
        }
};

vector<RGB*> BuildColorRGB(){
    Mat img;
    vector<RGB*> colors;
    VideoCapture cap;
    string urlPath = "https://rainsin-1305486451.file.myqcloud.com/rainsin-blog/img/%E6%9C%88%E4%BA%AE.jpg";
    cap.open(urlPath.c_str());

    cap >> img;

    if (img.empty()) {
        printf("图片读取失败");
        waitKey(0);
    }

    int height = img.rows;
    int width = img.cols;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            cv::Vec3b uc_pixel = img.at<cv::Vec3b>(row, col);
            int a = uc_pixel[0];
            int b = uc_pixel[1];
            int c = uc_pixel[2];
            RGB* rgb = new RGB(a,b,c);
            // cout << "像素:" << a << "," << b << "," << c <<endl;
            colors.push_back(rgb);
        }
    }

    return colors;
}