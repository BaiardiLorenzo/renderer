//
// Created by thomas on 13/12/2022.
//

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#include "test.h"
#include "values.h"


int main() {
    std::srand(time(nullptr));
    std::vector<cv::Mat> planes;
    cv::Mat base(MAX_ROWS, MAX_COLS, CV_8UC4, cv::Scalar(255, 255, 255, 0));
    for(int i=0; i<TEST5; i++)
        planes.push_back(cv::Mat(MAX_ROWS, MAX_COLS, CV_8UC4, cv::Scalar(255, 255, 255, 0)));

    double startTime = omp_get_wtime();

#pragma omp parallel shared(planes)
    {
#pragma omp for collapse(2)
        for (int i = 0; i < planes.size(); i++) {

            for (int j = 0; j < MAX_CIRCLES; j++) {
                int x = std::rand() % MAX_ROWS + 1;
                int y = std::rand() % MAX_COLS + 1;
                int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
                cv::Point center(x, y);
                cv::Scalar color(std::rand() % 255 + 1, std::rand() % 255 + 1, std::rand() % 255 + 1);
                cv::circle(planes[i], center, r, color, cv::FILLED, cv::LINE_AA);
            }
            printf("Piano %d\n", i + 1);
        }

#pragma omp single
        for (int i = 0; i < planes.size(); i++)
            cv::addWeighted(planes[i], ALPHA, base, 1 - ALPHA, 0, base);

        double time = omp_get_wtime() - startTime;
        std::cout << time << std::endl;

        cv::imshow("TEST", base);
        cv::waitKey(0);
    }
    return 0;
}
