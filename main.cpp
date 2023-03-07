#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <vector>
#include "main.h"
#ifdef _OPENMP
    #include <omp.h>
#endif

int main() {
#ifdef _OPENMP
    printf("OPENMP - core/threads processor: %d\n", omp_get_num_procs());
#endif
    std::vector<int> test_planes = {N_PLANES_100, N_PLANES_1000, N_PLANES_5000};
    Circle circles = generate_circles(N_CIRCLES);

    for (int test: test_planes) {
        //Initialization creation circles for all tests!!
        //Pass the circles on the functions
        renderer_seq(test, N_CIRCLES);
        renderer_circles_par(test, N_CIRCLES);
        renderer_planes_circles_par(test, N_CIRCLES);
    }
}

Circle generate_circles(int nCircle){
    Circle circles[nCircle];
    for(int i=0;i<nCircle;i++) {
        int x = std::rand() % HEIGHT + 1;
        int y = std::rand() % WIDTH + 1;
        cv::Point center(x,y);
        cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
        int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
        circles[i] = Circle{center, color, r};
    }
    return circles;
}

double renderer_seq(int n_planes, int n_circles) {
    printf("RENDERER SEQUENTIAL\n");
    std::srand(time(nullptr));
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);

    double startTime = omp_get_wtime();

    for (int i = 0; i < n_planes; i++) {
        cv::Mat plane;
        result.copyTo(plane);
        printf("PLANE: %d\n", i + 1);
        for (int j = 0; j < n_circles; j++) {
            int x = std::rand() % HEIGHT + 1;
            int y = std::rand() % WIDTH + 1;
            int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
            cv::Point center(x, y);
            cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
            cv::circle(plane, center, r, color, cv::FILLED, cv::LINE_AA);
        }
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);
    }

    double time = omp_get_wtime() - startTime;
    printf("TIME: %f\n", time);

    cv::imwrite("../img/seq_" + std::to_string(n_planes) + ".png", result);
    return time;
}

double renderer_circles_par(int n_planes, int n_circles) {
    printf("RENDERER CIRCLE PARALLEL\n");
    std::srand(time(nullptr));
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);

    double startTime = omp_get_wtime();

    for (int i = 0; i < n_planes; i++) {
        cv::Mat plane;
        result.copyTo(plane);
        printf("PLANE: %d\n", i + 1);
#pragma omp parallel for default(none) shared(plane, n_circles)
        for (int j = 0; j < n_circles; j++) {
            int x = std::rand() % HEIGHT + 1;
            int y = std::rand() % WIDTH + 1;
            int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
            cv::Point center(x, y);
            cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255);
            cv::circle(plane, center, r, color, cv::FILLED, cv::LINE_AA);
        }
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);
    }

    double time = omp_get_wtime() - startTime;
    printf("TIME: %f\n", time);

    cv::imwrite("../img/par_circles_" + std::to_string(n_planes) + ".png", result);
    return time;
}

double renderer_planes_circles_par(int n_planes, int n_circles) {
    printf("RENDERER PLANE AND CIRCLE PARALLEL\n");
    std::srand(time(nullptr));
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    std::vector<cv::Mat> planes;
    for (int i = 0; i < n_planes; i++)
        planes.push_back(cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT));

    double startTime = omp_get_wtime();

#pragma omp parallel for default(none) shared(planes, n_circles)
    for (int i = 0; i < planes.size(); i++) {
#pragma omp parallel for default(none) shared(n_circles, planes, i)
        for (int j = 0; j < n_circles; j++) {
            int x = std::rand() % HEIGHT + 1;
            int y = std::rand() % WIDTH + 1;
            int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
            cv::Point center(x, y);
            cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
            cv::circle(planes[i], center, r, color, cv::FILLED, cv::LINE_AA);
        }
        printf("PLANE: %d\n", i + 1);
    }
#pragma omp barrier
    for (const auto &plane: planes)
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);

    double time = omp_get_wtime() - startTime;
    printf("TIME: %f\n", time);

    cv::imwrite("../img/par_planes_circles_" + std::to_string(n_planes) + ".png", result);
    return time;
}