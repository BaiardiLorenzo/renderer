#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#include "values.h"


int mail() {
    std::srand(time(nullptr));
    cv::Mat base(MAX_ROWS, MAX_COLS, CV_8UC4, TRANSPARENT);

    double startTime = omp_get_wtime();

    for (int i = 0; i < N_PLANE; i++) {
        cv::Mat plane;
        base.copyTo(plane);
        printf("PLANE: %d\n", i + 1);
        for (int j = 0; j < N_CIRCLES; j++) {
            int x = std::rand() % MAX_ROWS + 1;
            int y = std::rand() % MAX_COLS + 1;
            int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
            cv::Point center(x, y);
            cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
            cv::circle(plane, center, r, color, cv::FILLED, cv::LINE_AA);
        }
        cv::addWeighted(plane, ALPHA, base, 1 - ALPHA, 0, base);
    }

    double time = omp_get_wtime() - startTime;
    printf("TIME: %f\n", time);

    cv::imshow("TEST", base);
    cv::waitKey(0);
    return 0;
}