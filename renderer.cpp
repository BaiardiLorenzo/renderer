#include <iostream>
#include "renderer.h"

void generateCircles(Circle circles[], unsigned long long n) {
    std::srand(777);
// PARALLEL GENERATION CIRCLES
#pragma omp parallel for default(none) shared(circles) firstprivate(n)
    for (int i = 0; i < n; i++) {
        cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255);
        cv::Point center(std::rand() % HEIGHT + 1, std::rand() % WIDTH + 1);
        int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
        circles[i] = {color, center, r};
    }
}

double rendererSequential(Circle circles[], unsigned long long nPlanes, unsigned long long nCircles) {
    printf("RENDERER SEQUENTIAL %llu: ", nPlanes);
    cv::Mat planes[nPlanes];

    // START
    double start = omp_get_wtime();

    for (int i = 0; i < nPlanes; i++) {
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i * nCircles + j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }

    cv::Mat result = combinePlanesSequential(planes, nPlanes);

    double time = omp_get_wtime() - start;
    // END

    printf(" TIME %f sec.\n", time);

    cv::imwrite("../img/seq_" + std::to_string(nPlanes) + ".png", result);
    return time;
}

cv::Mat combinePlanesSequential(cv::Mat planes[], unsigned long long nPlanes) {
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    int cn = result.channels();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for (int z = 0; z < nPlanes; z++) {
                cv::Mat *src2 = &planes[z];
                for (int c = 0; c < cn; c++)
                    result.data[i * result.step + cn * j + c] =
                            result.data[i * result.step + j * cn + c] * (1 - ALPHA) +
                            src2->data[i * src2->step + j * cn + c] * ALPHA;
            }
        }
    }
    return result;
}

double rendererParallel(Circle circles[], unsigned long long nPlanes, unsigned long long nCircles) {
    printf("RENDERER PARALLEL %llu: ", nPlanes);
    cv::Mat planes[nPlanes];

    double start = omp_get_wtime();

#pragma omp parallel for default(none) shared(planes, circles) firstprivate(nPlanes, nCircles)
    for (int i = 0; i < nPlanes; i++) {
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i * nCircles + j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }

    cv::Mat result = combinePlanesParallel(planes, nPlanes);

    double time = omp_get_wtime() - start;

    printf(" TIME %f sec.\n", time);

    cv::imwrite("../img/par_" + std::to_string(nPlanes) + ".png", result);
    return time;
}

cv::Mat combinePlanesParallel(cv::Mat planes[], unsigned long long nPlanes) {
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    int cn = result.channels();
#pragma omp parallel for collapse(2) default(none) shared(result, planes) firstprivate(nPlanes, cn)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for (int z = 0; z < nPlanes; z++) {
                cv::Mat *src2 = &planes[z];
                for (int c = 0; c < cn; c++)
                    result.data[i * result.step + cn * j + c] =
                            result.data[i * result.step + j * cn + c] * (1 - ALPHA) +
                            src2->data[i * src2->step + j * cn + c] * (ALPHA);
            }
        }
    }
    return result;
}

