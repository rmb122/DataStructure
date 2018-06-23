#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

void showImg(Mat& img) {
    namedWindow("Image", CV_WINDOW_AUTOSIZE);
    imshow("Image", img);
    waitKey(0);
}

inline int& overflow(int &x) {
    if(x > 255) {
        x = 255;
    }
    return x;
}

inline int& underflow(int &x) {
    if(x < 0) {
        x = 0;
    }
    return x;
}

Mat loadImg(char const* name) {
    Mat img = imread(name, CV_LOAD_IMAGE_UNCHANGED);
    if(img.channels() == 4) {
        img = imread(name, CV_LOAD_IMAGE_COLOR); //重新读取三通道的
    }
    return img;
}

Mat graify(Mat &img) {
    Mat newImg(img.rows, img.cols, CV_8UC1);
    for(unsigned int x = 0; x < img.cols; x++) {
        for(unsigned int y = 0; y < img.rows; y++) {
            newImg.at<uchar>(y, x) = (img.at<Vec3b>(y, x)[0] + img.at<Vec3b>(y, x)[1] + img.at<Vec3b>(y, x)[2]) / 3;
        }
    }
    return newImg;
}

Mat sharpfyRGB(Mat &img) {
    int laplaTemplate[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
    Mat newImg = img.clone();
    for(unsigned int x = 1; x < img.cols - 1; x++) {
        for(unsigned int y = 1; y < img.rows - 1; y++) {
            int R = 0, G = 0, B = 0;
            unsigned int index = 0;
            for(int xShift = -1; xShift < 2 ; xShift++) {
                for(int yShift = -1; yShift < 2 ; yShift++) {
                    R += (img.at<Vec3b>(y + yShift, x + xShift)[2] * laplaTemplate[index]);
                    G += (img.at<Vec3b>(y + yShift, x + xShift)[1] * laplaTemplate[index]);
                    B += (img.at<Vec3b>(y + yShift, x + xShift)[0] * laplaTemplate[index]);
                    index++;
                }
            }
            R = overflow(underflow(R));
            G = overflow(underflow(G));
            B = overflow(underflow(B));
            newImg.at<Vec3b>(y, x)[2] = R;
            newImg.at<Vec3b>(y, x)[1] = G;
            newImg.at<Vec3b>(y, x)[0] = B;
        }
    }
    return newImg;
}

Mat sharpfyGray(Mat &img) {
    int laplaTemplate[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
    Mat newImg = img.clone();
    for(unsigned int x = 1; x < img.cols - 1; x++) {
        for(unsigned int y = 1; y < img.rows - 1; y++) {
            int gray = 0;
            unsigned int index = 0;
            for(int xShift = -1; xShift < 2 ; xShift++) {
                for(int yShift = -1; yShift < 2 ; yShift++) {
                    gray += (img.at<uchar>(y + yShift, x + xShift) * laplaTemplate[index]);
                    index++;
                }
            }
            gray = overflow(underflow(gray));
            newImg.at<uchar>(y, x) = gray;
        }
    }
    return newImg;
}

Mat smoothfyRGB(Mat &img) {
    int gaussTemplate[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    Mat newImg = img.clone();
    for(unsigned int x = 1; x < img.cols - 1; x++) {
        for(unsigned int y = 1; y < img.rows - 1; y++) {
            int R = 0, G = 0, B = 0;
            unsigned int index = 0;
            for(int xShift = -1; xShift < 2 ; xShift++) {
                for(int yShift = -1; yShift < 2 ; yShift++) {
                    R += img.at<Vec3b>(y + yShift, x + xShift)[2] * gaussTemplate[index];
                    G += img.at<Vec3b>(y + yShift, x + xShift)[1] * gaussTemplate[index];
                    B += img.at<Vec3b>(y + yShift, x + xShift)[0] * gaussTemplate[index];
                    index++;
                }
            }
            R = R / 16;
            G = G / 16;
            B = B / 16;
            R = overflow(R);
            G = overflow(G);
            B = overflow(B);
            newImg.at<Vec3b>(y, x)[2] = R;
            newImg.at<Vec3b>(y, x)[1] = G;
            newImg.at<Vec3b>(y, x)[0] = B;
        }
    }
    return newImg;
}

Mat smoothfyGray(Mat &img) {
    int gaussTemplate[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    Mat newImg = img.clone();
    for(unsigned int x = 1; x < img.cols - 1; x++) {
        for(unsigned int y = 1; y < img.rows - 1; y++) {
            int gray = 0;
            unsigned int index = 0;
            for(int xShift = -1; xShift < 2 ; xShift++) {
                for(int yShift = -1; yShift < 2 ; yShift++) {
                    gray += (img.at<uchar>(y + yShift, x + xShift) * gaussTemplate[index]);
                    index++;
                }
            }
            gray = gray / 16;
            gray = overflow(gray);
            newImg.at<uchar>(y, x) = gray;
        }
    }
    return newImg;
}

Mat binaryzation(Mat &img) {
    std::vector<int> histogram(256, 0);
    unsigned int size = img.rows * img.cols; //图像大小
    unsigned int threshold = 0;
    double variance = 0;
    for(unsigned int x = 0; x < img.cols; ++x) { //生成直方图
        for(unsigned int y = 0; y < img.rows; ++y) {
            histogram[img.at<uchar>(y, x)]++;
        }
    }
    for(unsigned int i = 0 ; i < 256; ++i) {
        unsigned int bgPixCount = 0;
        unsigned int bgGraySum = 0;
        unsigned int frPixCount = 0;
        unsigned int frGraySum = 0;
        double bgGrayRatio = 0; //背景平均灰度
        double bgPixRatio = 0; //背景像素占所有像素的比例
        double frGrayRatio = 0;
        double frPixRatio = 0;
        unsigned int bgIndex = 0;
        for(; bgIndex < i; bgIndex++) {
            bgPixCount += histogram[bgIndex];
            bgGraySum += histogram[bgIndex] * bgIndex;
        }
        if(bgPixCount == 0) {
            continue;
        } else {
            bgPixRatio = static_cast<double>(bgPixCount) / size;
            bgGrayRatio = static_cast<double>(bgGraySum) / bgPixCount;
        }

        for(unsigned int frIndex = bgIndex; frIndex < 256; ++frIndex) {
            frPixCount += histogram[frIndex];
            frGraySum += histogram[frIndex] * frIndex;
        }
        if(frPixCount == 0) {
            continue;
        } else {
            frPixRatio = static_cast<double>(frPixCount) / size;
            frGrayRatio = static_cast<double>(frGraySum) / frPixCount;
        }
        double varianceTemp = frPixRatio * bgPixRatio * (frGrayRatio - bgGrayRatio) * (frGrayRatio - bgGrayRatio); //计算方差
        if(varianceTemp > variance) {
            variance = varianceTemp;
            threshold = i;
        }
    }
    for(unsigned int x = 0; x < img.cols; ++x) { //二值化
        for(unsigned int y = 0; y < img.rows; ++y) {
            if(img.at<uchar>(y, x) < threshold) {
                img.at<uchar>(y, x) = 0;
            } else {
                img.at<uchar>(y, x) = 255;
            }
        }
    }
    return img;
}

Mat edgeDetect(Mat &img) {
    int sobelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1} ;
    int sobelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1} ;
    Mat newImg = img.clone();

    for(unsigned int x = 1; x < img.cols - 1; x++) {
        for(unsigned int y = 1; y < img.rows - 1; y++) {
            int gx = 0;
            int gy = 0;
            int sum = 0;
            unsigned int index = 0;
            for(int xShift = -1; xShift < 2 ; xShift++) {
                for(int yShift = -1; yShift < 2 ; yShift++) {
                    gx += (img.at<uchar>(y + yShift, x + xShift) * sobelX[index]);
                    gy += (img.at<uchar>(y + yShift, x + xShift) * sobelY[index]);
                    index++;
                }
            }
            sum = abs(gx) + abs(gy);
            sum = overflow(underflow(sum));
            newImg.at<uchar>(y, x) = sum;
        }
    }
    binaryzation(newImg); //二值化
    return newImg;
}

int main(int argc, char const *argv[]) {
    if(argc == 4) { //mode path savePath
        char const* modeChar = argv[1];
        char const* path = argv[2];
        char const* savePath = argv[3];

        int mode = atoi(modeChar);
        Mat img = loadImg(path);

        switch(mode) {
        case 1:
            img = graify(img);
            break;
        case 2:
            img = sharpfyGray(img);
            break;
        case 3:
            img = sharpfyRGB(img);
            break;
        case 4:
            img = smoothfyGray(img);
            break;
        case 5:
            img = smoothfyRGB(img);
            break;
        case 6:
            img = binaryzation(img);
            break;
        case 7:
            img = edgeDetect(img);
            break;
        default:
            return -1;
        }
        imwrite(savePath, img);
        return 0;
    } else {
        return -1;
    }
}
