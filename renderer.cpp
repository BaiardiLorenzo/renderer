#include <iostream>
#include "renderer.h"

Circle* generateCircles(std::size_t n) {
    auto* circles = new Circle[n];
    std::srand(777);
#pragma omp parallel for default(none) shared(circles) firstprivate(n) // PARALLEL GENERATION CIRCLES
    for (int i = 0; i < n; i++) {
        cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255);
        cv::Point center(std::rand() % HEIGHT + 1, std::rand() % WIDTH + 1);
        int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
        circles[i] = {color, center, r};
    }
    return circles;
}

double rendererSequential(Circle circles[], std::size_t nPlanes, std::size_t nCircles) {
    auto* planes = new cv::Mat[nPlanes];

    // START
    double start = omp_get_wtime();

    for (int i = 0; i < nPlanes; i++) {
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for (int j = 0; j < nCircles; j++) {
            auto circle = circles[i * nCircles + j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }

    cv::Mat result = combinePlanesSequential(planes, nPlanes);

    double time = omp_get_wtime() - start;
    // END

    printf("Sequential time %f sec.\n", time);

    delete[] planes;

    cv::imwrite("../img/seq_" + std::to_string(nPlanes) + ".png", result);
    return time;
}

cv::Mat combinePlanesSequential(cv::Mat planes[], std::size_t nPlanes) {
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    std::size_t cn = result.channels();
    for (std::size_t i = 0; i < HEIGHT; i++) {
        for (std::size_t j = 0; j < WIDTH; j++) {
            for (std::size_t z = 0; z < nPlanes; z++) {
                cv::Mat *src2 = &planes[z];
                for (std::size_t c = 0; c < cn; c++)
                    result.data[i * result.step + cn * j + c] =
                            result.data[i * result.step + j * cn + c] * (1 - ALPHA) +
                            src2->data[i * src2->step + j * cn + c] * ALPHA;
            }
        }
    }
    return result;
}

double rendererParallel(Circle circles[], std::size_t nPlanes, std::size_t nCircles) {
    auto* planes = new cv::Mat[nPlanes];

    // START
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
    // END
    printf("Parallel time %f sec.\n", time);

    delete[] planes;

    cv::imwrite("../img/par_" + std::to_string(nPlanes) + ".png", result);
    return time;
}

cv::Mat combinePlanesParallel(cv::Mat planes[], std::size_t nPlanes) {
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    std::size_t cn = result.channels();
#pragma omp parallel for collapse(2) default(none) shared(result, planes) firstprivate(nPlanes, cn)
    for (std::size_t i = 0; i < HEIGHT; i++) {
        for (std::size_t j = 0; j < WIDTH; j++) {
            for (std::size_t z = 0; z < nPlanes; z++) {
                cv::Mat *src2 = &planes[z];
                for (std::size_t c = 0; c < cn; c++)
                    result.data[i * result.step + cn * j + c] =
                            result.data[i * result.step + j * cn + c] * (1 - ALPHA) +
                            src2->data[i * src2->step + j * cn + c] * (ALPHA);
            }
        }
    }
    return result;
}

