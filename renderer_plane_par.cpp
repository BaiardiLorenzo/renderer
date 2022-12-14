#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#include "values.h"


int main() {
    std::srand(time(nullptr));
    cv::Mat base(MAX_ROWS, MAX_COLS, CV_8UC4, TRANSPARENT);
    std::vector<cv::Mat> planes;
    for (int i=0; i < N_PLANE; i++)
        planes.push_back(cv::Mat(MAX_ROWS, MAX_COLS, CV_8UC4, TRANSPARENT));

    double startTime = omp_get_wtime();

#pragma omp parallel for default(none) shared(planes)
    for (int i = 0; i < planes.size(); i++) {
#pragma omp parallel for default(none) shared(planes, i)
        for (int j = 0; j < MAX_CIRCLES; j++) {
            int x = std::rand() % MAX_ROWS + 1;
            int y = std::rand() % MAX_COLS + 1;
            int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
            cv::Point center(x, y);
            cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
            cv::circle(planes[i], center, r, color, cv::FILLED, cv::LINE_AA);
        }
        printf("PLANE:%d\n", i + 1);
    }
#pragma omp barrier
    for (const auto & plane : planes)
        cv::addWeighted(plane, ALPHA, base, 1 - ALPHA, 0, base);

    double time = omp_get_wtime() - startTime;
    printf("TIME:%f\n", time);

    cv::imshow("TEST", base);
    cv::waitKey(0);
    return 0;
}
