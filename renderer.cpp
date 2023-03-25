#include "renderer.h"

void generateCircles(Circle circles[], unsigned long long n) {
    std::srand(0);
//Parallel generation circles
#pragma omp parallel for default(none) shared(circles, n)
    for (int i = 0; i < n; i++) {
        cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
        cv::Point center(std::rand() % HEIGHT + 1, std::rand() % WIDTH + 1);
        int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
        circles[i] = {color, center, r};
    }
}

double rendererSequential(Circle circles[], int nPlanes, int nCircles) {
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
        //cv::imshow("test", result);
        //cv::waitKey(0);
    }

    double time = omp_get_wtime() - start;

    printf("TIME: %f\n", time);

    cv::imwrite("../img/seq_" + std::to_string(nPlanes) + ".bmp", result);
    return time;
}

double rendererParallel(Circle circles[], int nPlanes, int nCircles) {
    printf("RENDERER PARALLEL\n");
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    cv::Mat planes[nPlanes];
    for (int i = 0; i < nPlanes; i++)
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);

    double start = omp_get_wtime();

#pragma omp parallel for default(none) shared(planes, nPlanes, circles, nCircles)
    for (int i = 0; i < nPlanes; i++) {
        printf("PLANE: %d\n", i + 1);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i*nCircles+j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }
#pragma omp barrier

    for (const auto &plane: planes) {
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);
        //cv::imshow("test", result);
        //cv::waitKey(0);
    }

    double time = omp_get_wtime() - start;
    printf("TIME: %f\n", time);
    cv::imwrite("../img/par_" + std::to_string(nPlanes) + ".bmp", result);
    return time;
}