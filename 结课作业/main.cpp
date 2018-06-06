#include <iostream>
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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

Mat loadImg(char name[]) {
    Mat img = imread(name, CV_LOAD_IMAGE_UNCHANGED);
    return img;
}

Mat graify(Mat &img) {
    Mat newImg(img.rows, img.cols, CV_8UC1);
    for(unsigned int x = 0; x < img.cols; x++) {
        for(unsigned int y = 0; y < img.rows; y++) {
            newImg.at<uchar>(y, x) = (img.at<Vec4b>(y, x)[0] + img.at<Vec4b>(y, x)[1] + img.at<Vec4b>(y, x)[2]) / 3;
        }
    }
    return newImg;
}

Mat sharpfyRGBA(Mat &img) {
    int laplaTemplate[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
    Mat newImg = img.clone();
    for(unsigned int x = 1; x < img.cols - 1; x++) {
        for(unsigned int y = 1; y < img.rows - 1; y++) {
            int R = 0, G = 0, B = 0;
            unsigned int index = 0;
            for(int xShift = -1; xShift < 2 ; xShift++) {
                for(int yShift = -1; yShift < 2 ; yShift++) {
                    R += (img.at<Vec4b>(y + yShift, x + xShift)[2] * laplaTemplate[index]);
                    G += (img.at<Vec4b>(y + yShift, x + xShift)[1] * laplaTemplate[index]);
                    B += (img.at<Vec4b>(y + yShift, x + xShift)[0] * laplaTemplate[index]);
                    index++;
                }
            }
            R = overflow(underflow(R));
            G = overflow(underflow(G));
            B = overflow(underflow(B));
            newImg.at<Vec4b>(y, x)[2] = R;
            newImg.at<Vec4b>(y, x)[1] = G;
            newImg.at<Vec4b>(y, x)[0] = B;
            newImg.at<Vec4b>(y, x)[3] = img.at<Vec4b>(y, x)[3];
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

Mat smoothfyRGBA(Mat &img) {
    int gaussTemplate[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    Mat newImg = img.clone();
    for(unsigned int x = 1; x < img.cols - 1; x++) {
        for(unsigned int y = 1; y < img.rows - 1; y++) {
            int R = 0, G = 0, B = 0;
            unsigned int index = 0;
            for(int xShift = -1; xShift < 2 ; xShift++) {
                for(int yShift = -1; yShift < 2 ; yShift++) {
                    R += img.at<Vec4b>(y + yShift, x + xShift)[2] * gaussTemplate[index];
                    G += img.at<Vec4b>(y + yShift, x + xShift)[1] * gaussTemplate[index];
                    B += img.at<Vec4b>(y + yShift, x + xShift)[0] * gaussTemplate[index];
                    index++;
                }
            }
            R = R / 16;
            G = G / 16;
            B = B / 16;
            R = overflow(R);
            G = overflow(G);
            B = overflow(B);
            newImg.at<Vec4b>(y, x)[2] = R;
            newImg.at<Vec4b>(y, x)[1] = G;
            newImg.at<Vec4b>(y, x)[0] = B;
            newImg.at<Vec4b>(y, x)[3] = img.at<Vec4b>(y, x)[3];
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

int main(int argc, char const *argv[]) {
    if(argc == 4) { //mode path savePath
        char const* modeChar = argv[1];
        char const* path = argv[2];
        char const* savePath = argv[3];

        int mode = atoi(modeChar);
        Mat img = imread(path, CV_LOAD_IMAGE_UNCHANGED);

        switch(mode) {
        case 1:
            img = graify(img);
            break;
        case 2:
            img = sharpfyGray(img);
            break;
        case 3:
            img = sharpfyRGBA(img);
            break;
        case 4:
            img = smoothfyGray(img);
            break;
        case 5:
            img = smoothfyRGBA(img);
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
