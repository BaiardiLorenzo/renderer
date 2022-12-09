#include <iostream>
#include <ctime>
#include <cstdlib>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#define TEST100 100
#define TEST1000 1000
#define TEST10000 10000

#define MAX_ROWS 500
#define MAX_COLS 500
#define MAX_RADIUS 50

#define ALPHA 0.3
#define BLUE cv::Scalar(178, 34, 34)
#define RED cv::Scalar(30, 144, 255)
#define YELLOW cv::Scalar(255, 255, 0)


int main() {
    std::srand(time(nullptr));
    cv::Mat matrix(MAX_ROWS, MAX_COLS, CV_8UC3, cv::Scalar(255, 255, 255)); //Declaring a matrix
    cv::Mat overlay;
    for(int i=0;i<TEST100;i++){
        int x = rand()%MAX_ROWS+1;
        int y = rand()%MAX_COLS+1;
        int r = rand()%MAX_RADIUS+1;
        cv::Point center(x, y); //Declaring the center point
        matrix.copyTo(overlay);
        switch (rand()%3+1) {
            case 1:
                cv::circle(matrix, center, r, BLUE, cv::FILLED);
                break;
            case 2:
                cv::circle(matrix, center, r, RED, cv::FILLED);
                break;
            case 3:
                cv::circle(matrix, center, r, YELLOW, cv::FILLED);
                break;
        }
        cv::addWeighted(overlay, ALPHA, matrix, 1 - ALPHA, 0, matrix);
    }
    cv::imshow("TEST", matrix);
    cv::waitKey(0);
    return 0;
}
