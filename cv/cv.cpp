#include <opencv2/opencv.hpp>
#include <vector>
#include <stack>
#include <random>

using namespace cv;

/*
 * 加载图像到内存
 */
Mat load_img(char const *name) {
    return imread(name, ImreadModes::IMREAD_COLOR);
}

/*
 * 保存内存中的图像到文件
 */
void write_img(Mat img, const char *filename) {
    imwrite(filename, img);
}

/*
 * 显示图片
 */
void show_img(Mat mat) {
    namedWindow("img", WINDOW_AUTOSIZE);
    moveWindow("img", 0, 0);
    imshow("img", mat);
    waitKey(0);
    destroyWindow("img");
}

/*
 * 灰度化图像, 按照 RGB 求平均值得到灰度值
 */
Mat graify_image(Mat &img) {
    Mat new_img(img.rows, img.cols, CV_8UC1);
    for (unsigned int x = 0; x < img.cols; x++) {
        for (unsigned int y = 0; y < img.rows; y++) {
            new_img.at<uchar>(y, x) = (img.at<Vec3b>(y, x)[0] + img.at<Vec3b>(y, x)[1] + img.at<Vec3b>(y, x)[2]) / 3; // 求平均值
        }
    }
    return new_img;
}

/*
 * 求图像的局部二值模式 (LBP) 特征
 */
Mat rilbp_feature(Mat &img) {
    Mat lbp_img = img.clone();
    unsigned char min_lbp_table[256];

    for (int i = 0; i < 256; i++) {
        unsigned char min = i;
        unsigned char temp = i;

        for (int j = 0; j < 7; j++) {
            bool lsb = temp & 1;
            temp = temp >> 1;
            if (lsb) {
                temp |= 128;
            } else {
                temp &= 127;
            }

            if (temp < min) {
                min = temp;
            }
        }
        min_lbp_table[i] = min; // 对每一个值循环八次打一张到最低值的表, 加快计算速度
    }

    for (int x = 1; x < img.cols - 1; x++) {
        for (int y = 1; y < img.rows - 1; y++) {
            uchar center = img.at<uchar>(y, x);
            uchar lbp_val = 0;
            lbp_val |= (img.at<uchar>(y - 1, x - 1) >= center) << 7; // 将像素点周围的 8 个像素转换为数字
            lbp_val |= (img.at<uchar>(y - 1, x) >= center) << 6;
            lbp_val |= (img.at<uchar>(y - 1, x + 1) >= center) << 5;
            lbp_val |= (img.at<uchar>(y, x + 1) >= center) << 4;
            lbp_val |= (img.at<uchar>(y + 1, x + 1) >= center) << 3;
            lbp_val |= (img.at<uchar>(y + 1, x) >= center) << 2;
            lbp_val |= (img.at<uchar>(y + 1, x - 1) >= center) << 1;
            lbp_val |= (img.at<uchar>(y, x - 1) >= center);
            lbp_img.at<uchar>(y, x) = min_lbp_table[lbp_val]; // 求最低的值, 达到旋转不变都目的
        }
    }

    return lbp_img;
}

/*
 * 求图像的 Hog 特征
 */
std::vector<std::vector<double>> hog_feature(Mat &img) {
    std::vector<std::vector<int>> x_direction(img.rows, std::vector<int>(img.cols));
    std::vector<std::vector<int>> y_direction(img.rows, std::vector<int>(img.cols));
    std::vector<std::vector<double>> gradient_val(img.rows, std::vector<double>(img.cols));
    std::vector<std::vector<double>> gradient_direction(img.rows, std::vector<double>(img.cols));
    std::vector<std::vector<int>> img_bin(img.rows, std::vector<int>(img.cols));

    auto deg2bin = [](double deg) -> int { // 定义度数到 bin 值的 lambda 函数
        for (int i = 0; i < 9; i++) {
            if ((deg >= 20 * i && deg < 20 + 20 * i) || (deg >= -180 + (20 * i) && deg < -160 + (20 * i))) {
                return i;
            }
        }
        if (round(deg) == 180) {
            return 8;
        }
        throw std::runtime_error("Error!");
    };

    for (int x = 1; x < img.cols - 1; x++) {
        for (int y = 0; y < img.rows; y++) {
            x_direction[y][x] = img.at<uchar>(y, x + 1) - img.at<uchar>(y, x - 1);
        }
    }

    for (int y = 0; y < img.rows; y++) {
        x_direction[y][0] = img.at<uchar>(y, 1) - img.at<uchar>(y, 0);
        x_direction[y][img.cols - 1] = img.at<uchar>(y, img.cols - 1) - img.at<uchar>(y, img.cols - 2);
    }

    for (int x = 0; x < img.cols; x++) {
        for (int y = 1; y < img.rows - 1; y++) {
            y_direction[y][x] = img.at<uchar>(y + 1, x) - img.at<uchar>(y - 1, x);
        }
    }

    for (int x = 0; x < img.cols; x++) {
        y_direction[0][x] = img.at<uchar>(1, x) - img.at<uchar>(0, x);
        y_direction[img.rows - 1][x] = img.at<uchar>(img.rows - 1, x) - img.at<uchar>(img.rows - 2, x);
    }

    for (int x = 0; x < img.cols; x++) {
        for (int y = 0; y < img.rows; y++) { // 求 bin 值和幅值
            gradient_val[y][x] = sqrt(x_direction[y][x] * x_direction[y][x] + y_direction[y][x] * y_direction[y][x]);
            gradient_direction[y][x] = (atan2(-y_direction[y][x], x_direction[y][x]) / CV_PI) * 180;
            img_bin[y][x] = deg2bin(gradient_direction[y][x]);
        }
    }

    const int cell_size = 4;
    const int block_size = 2;
    const int bin_size = 9;

    std::vector<std::vector<int>> feature_vector;
    std::vector<std::vector<double>> normed_block_feature_vector;

    for (int y = 0; y + (cell_size * block_size) <= img.rows; y += (cell_size * block_size)) {
        for (int x = 0; x + (cell_size * block_size) <= img.cols; x += (cell_size * block_size)) {
            std::vector<int> block_feature(bin_size * (block_size * block_size), 0);

            for (int x_block_shift = 0; x_block_shift < block_size; x_block_shift++) {
                for (int y_block_shift = 0; y_block_shift < block_size; y_block_shift++) {
                    for (int x_cell_shift = 0; x_cell_shift < cell_size; x_cell_shift++) {
                        for (int y_cell_shift = 0; y_cell_shift < cell_size; y_cell_shift++) {
                            int curr_y = y + y_cell_shift + y_block_shift * cell_size;
                            int curr_x = x + x_cell_shift + x_block_shift * cell_size;
                            block_feature[(y_block_shift * block_size + x_block_shift) * bin_size +
                                          img_bin[curr_y][curr_x]] += gradient_val[curr_y][curr_x]; // 将 cell 的幅值保存在 vector 里面
                        }
                    }
                }
            }
            feature_vector.push_back(block_feature);
        }
    }

    for (std::vector<int> &block_feature : feature_vector) {
        normed_block_feature_vector.emplace_back(block_feature.size(), 0);

        int sum = 0;
        for (int &i : block_feature) {
            sum += i;
        }

        for (int i = 0; i < block_feature.size(); i++) {
            normed_block_feature_vector.back()[i] = (double) block_feature[i] / sum; // 1-norm 化
        }
    }
    return normed_block_feature_vector;
}

/*
 * 将 hog 特征可视化
 */
Mat draw_hog_feature(std::vector<std::vector<double>> normed_block_feature_vector, int width, int height) {
    int cell_size = 4;
    int block_size = 2;
    int bin_size = 9;
    int scale_para = 21; // 将可视化图像等比例放大, 便于画向量线

    int img_width = (width - (width % (cell_size * block_size))) * scale_para;
    int img_height = (height - (height % (cell_size * block_size))) * scale_para;

    Mat img = Mat(Size(img_width, img_height), CV_8UC3, Scalar(0, 0, 0));
    int block_x = 0;
    int block_y = 0;

    for (std::vector<double> &block : normed_block_feature_vector) {
        int cell_x = 0;
        int cell_y = 0;

        for (int i = 0; i < block_size * block_size; i++) {
            std::vector<double> cell_feature(bin_size);
            std::copy(block.begin() + i * bin_size, block.begin() + (i + 1) * bin_size, cell_feature.begin());

            switch (i) {
                case 0: // 对 block 内的四个 cell 放在不同的地方绘图
                    cell_x = block_x;
                    cell_y = block_y;
                    break;
                case 1:
                    cell_x = block_x + cell_size * scale_para;
                    cell_y = block_y;
                    break;
                case 2:
                    cell_x = block_x;
                    cell_y = block_y + cell_size * scale_para;
                    break;
                case 3:
                    cell_x = block_x + cell_size * scale_para;
                    cell_y = block_y + cell_size * scale_para;
                    break;
                default:
                    throw std::runtime_error("Error");
            }
            cell_x += ((cell_size * scale_para) / 2 + 1);
            cell_y += ((cell_size * scale_para) / 2 + 1);

            double max = 0;
            for (double &iter : cell_feature) {
                if (iter > max) {
                    max = iter;
                }
            }

            for (double &iter : cell_feature) {
                iter = iter / max;
            }

            for (int bin_index = 0; bin_index < bin_size; bin_index++) { // 对 9 个 bin 画 9 个向量
                int x_shift = 0;
                int y_shift = 0;
                double bin_rad = (((double) bin_index * 20) / 180) * CV_PI;
                double length = cell_size * scale_para / 2;
                x_shift = round(length * cos(bin_rad) * cell_feature[bin_index]);
                y_shift = round(length * sin(bin_rad) * cell_feature[bin_index]);

                line(img, Point(cell_x + x_shift, cell_y + y_shift), Point(cell_x - x_shift, cell_y - y_shift),
                     Scalar(255, 255, 255)); // 在图上画线, 达成可视化目的
            }
            //show_img(img);
        }

        block_x += block_size * cell_size * scale_para;
        if (block_x == img_width) {
            block_x = 0;
            block_y += block_size * cell_size * scale_para;
        }
    }
    return img;
}

int main() {
    Mat raw_mat = load_img("/home/rmb122/temp/cv/32x32-3.png");
    Mat gray_mat = graify_image(raw_mat);
    //Mat mat = draw_hog_feature(hog_feature(gray_mat), gray_mat.cols, gray_mat.rows);
    Mat mat = rilbp_feature(gray_mat);

    show_img(mat);
    return 0;
}
