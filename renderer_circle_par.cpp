#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#include "test.h"
#include "values.h"


int maic() {
    std::srand(time(nullptr));
    cv::Mat base(MAX_ROWS, MAX_COLS, CV_8UC4, cv::Scalar(255, 255, 255, 0));

    double startTime = omp_get_wtime();

    for (int i = 0; i < TEST5; i++) {
        cv::Mat plane(MAX_ROWS, MAX_COLS, CV_8UC4, cv::Scalar(255, 255, 255, 0));
        base.copyTo(plane);
        printf("Piano %d\n", i + 1);

#pragma omp parallel
        {
#pragma omp for
            for (int j = 0; j < MAX_CIRCLES; j++) {
                int x = std::rand() % MAX_ROWS + 1;
                int y = std::rand() % MAX_COLS + 1;
                int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
                cv::Point center(x, y);
                cv::Scalar color(std::rand() % 255 + 1, std::rand() % 255 + 1, std::rand() % 255 + 1);
                cv::circle(plane, center, r, color, cv::FILLED, cv::LINE_AA);
            }
        }
        cv::addWeighted(plane, ALPHA, base, 1 - ALPHA, 0, base);
    }

    double endTime = omp_get_wtime();
    double time = endTime - startTime;
    std::cout<<time<<std::endl;

    cv::imshow("TEST", base);
    cv::waitKey(0);
    return 0;
}
