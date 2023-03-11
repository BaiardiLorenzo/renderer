#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <vector>

#ifdef _OPENMP

#include <omp.h>

#endif

#define WIDTH 500
#define HEIGHT 500
#define TRANSPARENT cv::Scalar(255, 255, 255, 0)
#define ALPHA 0.3
#define N_PLANES_100 100
#define N_PLANES_1000 1000
#define N_PLANES_5000 5000

#define MAX_RADIUS 70
#define MIN_RADIUS 10
#define N_CIRCLES 2000

struct Circle {
    cv::Scalar color;
    cv::Point center;
    int r = 0;
};

void generate_circles(Circle circles[], int nCircle, int idPlane) {
    std::srand(idPlane);
    for (int i = 0; i < nCircle; i++) {
        int x = std::rand() % HEIGHT + 1;
        int y = std::rand() % WIDTH + 1;
        cv::Point center(x, y);
        cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
        int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
        circles[i] = {color, center, r};
    }
}

double renderer_seq(int nPlanes, int nCircles) {
    printf("RENDERER SEQUENTIAL\n");
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);

    double start = omp_get_wtime();

    for (int i = 0; i < nPlanes; i++) {
        cv::Mat plane;
        result.copyTo(plane);
        printf("PLANE: %d\n", i + 1);
        Circle circles[N_CIRCLES];
        generate_circles(circles, N_CIRCLES, i);
        for (int j = 0; j < nCircles; j++)
            cv::circle(plane, circles[j].center, circles[j].r, circles[j].color, cv::FILLED, cv::LINE_AA);
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);
    }

    double time = omp_get_wtime() - start;

    printf("TIME: %f\n", time);

    cv::imwrite("../img/seq_" + std::to_string(nPlanes) + ".bmp", result);
    return time;
}

double renderer_par(int nPlanes, int nCircles) {
    printf("RENDERER PARALLEL\n");
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    std::vector<cv::Mat> planes;
    for (int i = 0; i < nPlanes; i++)
        planes.emplace_back(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);

    double start = omp_get_wtime();

#pragma omp parallel default(none) shared(planes, nCircles)
#pragma omp for
    for (int i = 0; i < planes.size(); i++) {
        Circle circles[N_CIRCLES];
        generate_circles(circles, N_CIRCLES, i);
        for (int j = 0; j < nCircles; j++)
            cv::circle(planes[i], circles[j].center, circles[j].r, circles[j].color, cv::FILLED, cv::LINE_AA);
        printf("PLANE: %d\n", i + 1);
    }

    for (const auto &plane: planes)
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);

    double time = omp_get_wtime() - start;
    printf("TIME: %f\n", time);
    cv::imwrite("../img/par" + std::to_string(nPlanes) + ".bmp", result);
    return time;
}

int main() {
#ifdef _OPENMP
    printf("Core/Threads: %d\n", omp_get_num_procs());
#endif
    std::vector<int> test_planes = {N_PLANES_100, N_PLANES_1000, N_PLANES_5000};

    for (int test: test_planes) {
        renderer_seq(test, N_CIRCLES);
        renderer_par(test, N_CIRCLES);
    }
}

