#include <opencv2/opencv.hpp>
#include <vector>
#include <stack>
#include <random>

using namespace cv;

Mat load_img(char const *name) {
    return imread(name, ImreadModes::IMREAD_COLOR);
}

void write_img(Mat img, const char *filename) {
    imwrite(filename, img);
}

void show_img(Mat mat) {
    namedWindow("img", WINDOW_AUTOSIZE);
    moveWindow("img", 0, 0);
    imshow("img", mat);
    waitKey(0);
    destroyWindow("img");
}

inline double &overflow(double &x) {
    if (x > 255) {
        x = 255;
    }
    return x;
}

inline double &underflow(double &x) {
    if (x < 0) {
        x = 0;
    }
    return x;
}

inline int &overflow(int &x) {
    if (x > 255) {
        x = 255;
    }
    return x;
}

inline int &underflow(int &x) {
    if (x < 0) {
        x = 0;
    }
    return x;
}

Mat gauss_filter(Mat &img) {
    double gauss_template[] = {
            0.0947416, 0.1183180, 0.0947416,
            0.1183180, 0.1477610, 0.1183180,
            0.0947416, 0.1183180, 0.0947416
    };

    Mat new_img = img.clone();

    for (int x = 1; x < img.cols - 1; x++) {
        for (int y = 1; y < img.rows - 1; y++) {
            double R = 0, G = 0, B = 0;
            unsigned int index = 0;
            for (int x_shift = -1; x_shift < 2; x_shift++) {
                for (int y_shift = -1; y_shift < 2; y_shift++) {
                    R += img.at<Vec3b>(y + y_shift, x + x_shift)[2] * gauss_template[index];
                    G += img.at<Vec3b>(y + y_shift, x + x_shift)[1] * gauss_template[index];
                    B += img.at<Vec3b>(y + y_shift, x + x_shift)[0] * gauss_template[index];
                    index++;
                }
            }

            R = overflow(R);
            G = overflow(G);
            B = overflow(B);
            new_img.at<Vec3b>(y, x)[2] = R;
            new_img.at<Vec3b>(y, x)[1] = G;
            new_img.at<Vec3b>(y, x)[0] = B;
        }
    }
    return new_img;
}

Mat average_filter(Mat &img) {
    Mat new_img = img.clone();

    for (int x = 1; x < img.cols - 1; x++) {
        for (int y = 1; y < img.rows - 1; y++) {
            double R = 0, G = 0, B = 0;
            unsigned int index = 0;
            for (int x_shift = -1; x_shift < 2; x_shift++) {
                for (int y_shift = -1; y_shift < 2; y_shift++) {
                    R += img.at<Vec3b>(y + y_shift, x + x_shift)[2];
                    G += img.at<Vec3b>(y + y_shift, x + x_shift)[1];
                    B += img.at<Vec3b>(y + y_shift, x + x_shift)[0];
                    index++;
                }
            }

            R = R / 9;
            G = G / 9;
            B = B / 9;

            R = overflow(R);
            G = overflow(G);
            B = overflow(B);
            new_img.at<Vec3b>(y, x)[2] = R;
            new_img.at<Vec3b>(y, x)[1] = G;
            new_img.at<Vec3b>(y, x)[0] = B;
        }
    }
    return new_img;
}

Mat median_filter(Mat &img) {
    Mat new_img = img.clone();
    std::vector<double> r_val_tmp(9);
    std::vector<double> g_val_tmp(9);
    std::vector<double> b_val_tmp(9);

    for (int x = 1; x < img.cols - 1; x++) {
        for (int y = 1; y < img.rows - 1; y++) {
            unsigned int index = 0;
            for (int x_shift = -1; x_shift < 2; x_shift++) {
                for (int y_shift = -1; y_shift < 2; y_shift++) {
                    r_val_tmp[(x_shift + 1) * 3 + (y_shift + 1)] = img.at<Vec3b>(y + y_shift, x + x_shift)[2];
                    g_val_tmp[(x_shift + 1) * 3 + (y_shift + 1)] = img.at<Vec3b>(y + y_shift, x + x_shift)[1];
                    b_val_tmp[(x_shift + 1) * 3 + (y_shift + 1)] = img.at<Vec3b>(y + y_shift, x + x_shift)[0];
                    index++;
                }
            }

            std::sort(r_val_tmp.begin(), r_val_tmp.end());
            std::sort(g_val_tmp.begin(), g_val_tmp.end());
            std::sort(b_val_tmp.begin(), b_val_tmp.end());

            double R = r_val_tmp[4];
            double G = g_val_tmp[4];
            double B = b_val_tmp[4];

            R = overflow(R);
            G = overflow(G);
            B = overflow(B);
            new_img.at<Vec3b>(y, x)[2] = R;
            new_img.at<Vec3b>(y, x)[1] = G;
            new_img.at<Vec3b>(y, x)[0] = B;
        }
    }
    return new_img;
}

Mat salt_image(Mat &img) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 100);

    Mat new_img = img.clone();

    for (int x = 1; x < img.cols - 1; x++) {
        for (int y = 1; y < img.rows - 1; y++) {
            if (distribution(generator) < 1) {
                new_img.at<Vec3b>(y, x)[2] = 255;
                new_img.at<Vec3b>(y, x)[1] = 255;
                new_img.at<Vec3b>(y, x)[0] = 255;
            }
        }
    }
    return new_img;
}

Mat graify_image(Mat &img) {
    Mat new_img(img.rows, img.cols, CV_8UC1);
    for (unsigned int x = 0; x < img.cols; x++) {
        for (unsigned int y = 0; y < img.rows; y++) {
            new_img.at<uchar>(y, x) = (img.at<Vec3b>(y, x)[0] + img.at<Vec3b>(y, x)[1] + img.at<Vec3b>(y, x)[2]) / 3;
        }
    }
    return new_img;
}

Mat equalization(Mat &img) {
    Mat new_img = img.clone();
    std::vector<int> gray_count(256, 0);
    std::vector<int> gray_cdf_count(256, 0);
    std::vector<int> gray_mapping(256, 0);

    for (unsigned int x = 0; x < img.cols; x++) {
        for (unsigned int y = 0; y < img.rows; y++) {
            gray_count[img.at<uchar>(y, x)]++;
        }
    }

    int sum = 0;
    int cdf_min = 0;
    int cdf_max = 0;

    for (int i = 0; i < gray_count.size(); i++) {
        sum += gray_count[i];
        gray_cdf_count[i] = sum;
    }

    for (int i = 0; i < gray_count.size(); i++) {
        if (gray_cdf_count[i] != 0) {
            cdf_min = gray_cdf_count[i];
            break;
        }
    }

    cdf_max = gray_cdf_count.back();

    for (int i = 0; i < gray_count.size(); i++) {
        double temp = 0.0;
        temp = ((double) gray_cdf_count[i] - (double) cdf_min) / ((img.cols * img.rows) - cdf_min);
        gray_mapping[i] = temp * 255;
    }

    for (unsigned int x = 0; x < img.cols; x++) {
        for (unsigned int y = 0; y < img.rows; y++) {
            new_img.at<uchar>(y, x) = gray_mapping[new_img.at<uchar>(y, x)];
        }
    }
    return new_img;
}

Mat binaryzation(Mat &img) {
    std::vector<int> histogram(256, 0);
    unsigned int size = img.rows * img.cols; //图像大小
    unsigned int threshold = 0;
    double variance = 0;

    for (unsigned int x = 0; x < img.cols; ++x) { //生成直方图
        for (unsigned int y = 0; y < img.rows; ++y) {
            histogram[img.at<uchar>(y, x)]++;
        }
    }

    for (unsigned int i = 0; i < 256; ++i) {
        unsigned int bg_pix_count = 0;
        unsigned int bg_gray_sum = 0;
        unsigned int fr_pix_count = 0;
        unsigned int fr_gray_sum = 0;
        double bg_gray_ratio = 0; //背景平均灰度
        double bg_pix_ratio = 0; //背景像素占所有像素的比例
        double fr_gray_ratio = 0;
        double fr_pix_ratio = 0;
        unsigned int bg_index = 0;

        for (; bg_index < i; bg_index++) {
            bg_pix_count += histogram[bg_index];
            bg_gray_sum += histogram[bg_index] * bg_index;
        }

        if (bg_pix_count == 0) {
            continue;
        } else {
            bg_pix_ratio = static_cast<double>(bg_pix_count) / size;
            bg_gray_ratio = static_cast<double>(bg_gray_sum) / bg_pix_count;
        }

        for (unsigned int frIndex = bg_index; frIndex < 256; ++frIndex) {
            fr_pix_count += histogram[frIndex];
            fr_gray_sum += histogram[frIndex] * frIndex;
        }
        if (fr_pix_count == 0) {
            continue;
        } else {
            fr_pix_ratio = static_cast<double>(fr_pix_count) / size;
            fr_gray_ratio = static_cast<double>(fr_gray_sum) / fr_pix_count;
        }
        double variance_temp =
                fr_pix_ratio * bg_pix_ratio * (fr_gray_ratio - bg_gray_ratio) * (fr_gray_ratio - bg_gray_ratio); //计算方差
        if (variance_temp > variance) {
            variance = variance_temp;
            threshold = i;
        }
    }

    for (unsigned int x = 0; x < img.cols; ++x) { //二值化
        for (unsigned int y = 0; y < img.rows; ++y) {
            if (img.at<uchar>(y, x) < threshold) {
                img.at<uchar>(y, x) = 0;
            } else {
                img.at<uchar>(y, x) = 255;
            }
        }
    }
    return img;
}

Mat edge_detect(Mat &img) {
    int sobel_X[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int sobel_Y[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    Mat new_img = img.clone();

    for (unsigned int x = 1; x < img.cols - 1; x++) {
        for (unsigned int y = 1; y < img.rows - 1; y++) {
            int gx = 0;
            int gy = 0;
            int sum = 0;
            unsigned int index = 0;
            for (int x_shift = -1; x_shift < 2; x_shift++) {
                for (int y_shift = -1; y_shift < 2; y_shift++) {
                    gx += (img.at<uchar>(y + y_shift, x + x_shift) * sobel_X[index]);
                    gy += (img.at<uchar>(y + y_shift, x + x_shift) * sobel_Y[index]);
                    index++;
                }
            }
            sum = (int) sqrt((gx * gx) + (gy * gy));
            sum = overflow(underflow(sum));
            new_img.at<uchar>(y, x) = sum;
        }
    }
    new_img = binaryzation(new_img); //二值化
    return new_img;
}

void hough_transform(Mat &img, Mat &raw_img) {
    unsigned int theta_granularity = 180;
    Mat new_img = img.clone();
    std::vector<std::vector<unsigned int>> accu; // <r, theta>
    double max_rho = sqrt((img.rows * img.rows) + (img.cols * img.cols));
    for (int i = 0; i < ((max_rho + 1) * 2); i++) {
        accu.emplace_back(theta_granularity, 0);
    }

    for (unsigned int x = 0; x < img.cols; x++) {
        for (unsigned int y = 0; y < img.rows; y++) {
            if (img.at<uchar>(y, x) == 255) {
                for (int theta_div = 0; theta_div < theta_granularity; theta_div++) {
                    double theta = (CV_PI * (double) theta_div / (double) theta_granularity);
                    int rho = (int) round(cos(theta) * (double) x + sin(theta) * (double) y + max_rho);
                    accu[rho][theta_div] += 1;
                }
            }
        }
    }

    std::vector<unsigned int> max(6, 0);

    for (unsigned int rho = 0; rho < ((max_rho + 1) * 2); rho++) {
        for (unsigned int theta = 0; theta < theta_granularity; theta++) {
            unsigned int curr = accu[rho][theta];
            unsigned int min_index = -1;

            for (unsigned int i = 0; i < max.size(); i++) {
                if (curr > max[i]) {
                    min_index = i;
                    break;
                }
            }
            if (min_index != -1) {
                max[min_index] = curr;
            }
        }
    }

    for (unsigned int rho = 0; rho < ((max_rho + 1) * 2); rho++) {
        for (unsigned int theta = 0; theta < theta_granularity; theta++) {
            Point point_one;
            Point point_two;

            bool in_max = false;

            for (unsigned int &tmp : max) {
                if (tmp == accu[rho][theta]) {
                    in_max = true;
                }
            }

            if (in_max) {
                int rho_tmp = rho - round(max_rho);
                if (theta > 0 && theta < 90) {
                    point_one.x = ((double) rho_tmp) / cos(((double) theta / (double) theta_granularity) * CV_PI);
                    point_one.y = 0;

                    point_two.x = 0;
                    point_two.y =
                            ((double) rho_tmp) / cos(((double) (90 - theta) / (double) theta_granularity) * CV_PI);
                } else if (theta == 0) {
                    point_one.x = rho_tmp;
                    point_one.y = 0;

                    point_two.x = rho_tmp;
                    point_two.y = img.rows;
                } else if (theta > 90 && rho_tmp > 0.0) {
                    unsigned int theta_tmp = 180 - theta;
                    point_one.x = 0;
                    double tmp =
                            ((double) rho_tmp) / cos(((double) (90 - theta_tmp) / (double) theta_granularity) * CV_PI);
                    point_one.y = tmp;

                    point_two.x = ((double) rho_tmp) / cos(((double) theta_tmp / (double) theta_granularity) * CV_PI) *
                                  ((double) img.rows / tmp);
                    point_two.y = img.rows;
                } else if (theta == 90) {
                    point_one.x = 0;
                    point_one.y = rho_tmp;

                    point_two.x = img.cols;
                    point_two.y = rho_tmp;
                } else if (theta > 90 && rho_tmp < 0.0) {
                    unsigned int theta_tmp = 180 - theta;
                    point_one.x = ((double) -rho_tmp) / cos(((double) theta_tmp / (double) theta_granularity) * CV_PI);
                    point_one.y = 0;

                    double tmp =
                            ((double) -rho_tmp) / cos(((double) (90 - theta_tmp) / (double) theta_granularity) * CV_PI);
                    point_two.x = point_one.x * ((double) (img.rows + tmp) / tmp);
                    point_two.y = img.rows;
                } else if (rho_tmp == 0) {
                    continue;
                } else {
                    throw std::runtime_error("Unexpected error");
                }
                //std::cout << rho_tmp << " " << theta << " " << accu[rho][theta] << std::endl;
                line(raw_img, point_one, point_two, cv::Scalar(255, 0, 0));
                //show_img(raw_img);
            }
        }
    }
}

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
        min_lbp_table[i] = min;
    }

    for (int x = 1; x < img.cols - 1; x++) {
        for (int y = 1; y < img.rows - 1; y++) {
            uchar center = img.at<uchar>(y, x);
            uchar lbp_val = 0;
            lbp_val |= (img.at<uchar>(y - 1, x - 1) >= center) << 7;
            lbp_val |= (img.at<uchar>(y - 1, x) >= center) << 6;
            lbp_val |= (img.at<uchar>(y - 1, x + 1) >= center) << 5;
            lbp_val |= (img.at<uchar>(y, x + 1) >= center) << 4;
            lbp_val |= (img.at<uchar>(y + 1, x + 1) >= center) << 3;
            lbp_val |= (img.at<uchar>(y + 1, x) >= center) << 2;
            lbp_val |= (img.at<uchar>(y + 1, x - 1) >= center) << 1;
            lbp_val |= (img.at<uchar>(y, x - 1) >= center);
            lbp_img.at<uchar>(y, x) = min_lbp_table[lbp_val];
        }
    }

    return lbp_img;
}

std::vector<std::vector<double>> hog_feature(Mat &img) {
    std::vector<std::vector<int>> x_direction(img.rows, std::vector<int>(img.cols));
    std::vector<std::vector<int>> y_direction(img.rows, std::vector<int>(img.cols));
    std::vector<std::vector<double>> gradient_val(img.rows, std::vector<double>(img.cols));
    std::vector<std::vector<double>> gradient_direction(img.rows, std::vector<double>(img.cols));
    std::vector<std::vector<int>> img_bin(img.rows, std::vector<int>(img.cols));

    auto deg2bin = [](double deg) -> int {
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
        for (int y = 0; y < img.rows; y++) {
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
                                          img_bin[curr_y][curr_x]] += gradient_val[curr_y][curr_x];
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
            normed_block_feature_vector.back()[i] = (double) block_feature[i] / sum;
        }
    }
    return normed_block_feature_vector;
}

Mat draw_hog_feature(std::vector<std::vector<double>> normed_block_feature_vector, int width, int height) {
    int cell_size = 4;
    int block_size = 2;
    int bin_size = 9;
    int scale_para = 21;

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
                case 0:
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

            for (int bin_index = 0; bin_index < bin_size; bin_index++) {
                int x_shift = 0;
                int y_shift = 0;
                double bin_rad = (((double) bin_index * 20) / 180) * CV_PI;
                double length = cell_size * scale_para / 2;
                x_shift = round(length * cos(bin_rad) * cell_feature[bin_index]);
                y_shift = round(length * sin(bin_rad) * cell_feature[bin_index]);

                line(img, Point(cell_x + x_shift, cell_y + y_shift), Point(cell_x - x_shift, cell_y - y_shift),
                     Scalar(255, 255, 255));
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
