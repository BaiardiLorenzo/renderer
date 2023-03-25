#include <iostream>
#include <cstdlib>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <vector>
#include <omp.h>

#define WIDTH 500
#define HEIGHT 500
#define TRANSPARENT cv::Scalar(255, 255, 255, 0)
#define ALPHA 0.3

#define MAX_RADIUS 70
#define MIN_RADIUS 10

#define N_PLANES_1 1
#define N_PLANES_2 20
#define N_CIRCLES 2

struct Circle {
    cv::Scalar color;
    cv::Point center;
    int r;
};

void generate_circles(Circle circles[], unsigned long long n) {
    std::srand(0);
    for (int i = 0; i < n; i++) {
        cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256, 0);
        cv::Point center(std::rand() % HEIGHT + 1, std::rand() % WIDTH + 1);
        int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
        circles[i] = {color, center, r};
    }
}



double renderer_seq(Circle circles[], int nPlanes, int nCircles) {
    printf("RENDERER SEQUENTIAL\n");
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);

    double start = omp_get_wtime();

    for (int i = 0; i < nPlanes; i++) {
        cv::Mat plane;
        result.copyTo(plane);
        printf("PLANE: %d\n", i + 1);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i*nCircles+j];
            cv::circle(plane, circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);
        cv::imshow("test", result);
        cv::waitKey(0);
    }

    double time = omp_get_wtime() - start;

    printf("TIME: %f\n", time);

    cv::imwrite("../img/seq_" + std::to_string(nPlanes) + ".bmp", result);
    return time;
}

double renderer_par(Circle circles[], int nPlanes, int nCircles) {
    printf("RENDERER PARALLEL\n");
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    std::vector<cv::Mat> planes;
    for (int i = 0; i < nPlanes; i++)
        planes.emplace_back(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);

    double start = omp_get_wtime();

#pragma omp parallel default(none) shared(planes, circles, nCircles)
#pragma omp for
    for (int i = 0; i < planes.size(); i++) {
        printf("PLANE: %d\n", i + 1);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i*nCircles+j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }
#pragma omp barrier

    for (const auto &plane: planes) {
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);
        cv::imshow("test", result);
        cv::waitKey(0);
    }

    double time = omp_get_wtime() - start;
    printf("TIME: %f\n", time);
    cv::imwrite("../img/par_" + std::to_string(nPlanes) + ".bmp", result);
    return time;
}

int main() {
#ifdef _OPENMP
    int threads = omp_get_num_procs();
    printf("Core/Threads: %d\n", threads);
    omp_set_dynamic(0);
#endif
    std::vector<double> sequential_times;
    std::vector<double> parallel_times;

    std::vector<int> test_planes = {N_PLANES_1 * threads, N_PLANES_2 * threads};

    for (int test: test_planes) {
        const unsigned long long n = test*N_CIRCLES;
        Circle circles[n];
        generate_circles(circles, n);
        double t1 = renderer_seq(circles, test, N_CIRCLES);
        double t2 = renderer_par(circles, test, N_CIRCLES);
        sequential_times.push_back(t1);
        parallel_times.push_back(t2);
    }


}

