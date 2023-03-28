#include "renderer.h"

void generateCircles(Circle circles[], unsigned long long n) {
    std::srand(0);
//Parallel generation circles
#pragma omp parallel for default(none) shared(circles) firstprivate(n)
    for (int i = 0; i < n; i++) {
        cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255);
        cv::Point center(std::rand() % HEIGHT + 1, std::rand() % WIDTH + 1);
        int r = std::rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS + 1;
        circles[i] = {color, center, r};
    }
}

double rendererSequential(Circle circles[], unsigned long long nPlanes, unsigned long long nCircles) {
    printf("RENDERER SEQUENTIAL\n");
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    cv::Mat planes[nPlanes];
    double start = omp_get_wtime();

    for (int i = 0; i < nPlanes; i++) {
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i*nCircles+j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }

    for (const auto &plane: planes)
        cv::addWeighted(plane, ALPHA, result, 1 - ALPHA, 0, result);

    double time = omp_get_wtime() - start;

    printf("TIME SEQ %llu: %f\n", nPlanes, time);

    cv::imwrite("../img/seq_" + std::to_string(nPlanes) + ".png", result);
    return time;
}

double rendererParallel(Circle circles[], unsigned long long nPlanes, unsigned long long nCircles) {
    printf("RENDERER PARALLEL\n");
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    cv::Mat planes[nPlanes];

    double start = omp_get_wtime();

#pragma omp parallel for default(none) shared(planes) firstprivate(nPlanes, circles, nCircles)
    for (int i = 0; i < nPlanes; i++) {
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i*nCircles+j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }

    int numProc = omp_get_num_procs();
    cv::Mat multiplePlanes[numProc];

#pragma omp parallel for default(none) shared(multiplePlanes) firstprivate(nPlanes, planes, nCircles, numProc)
    for(int i=0; i<numProc; i++) {
        cv::Mat chunkPlanes(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for(int j=0; j<(nPlanes/numProc); j++){
            cv::addWeighted(planes[i*(nPlanes/numProc)+j], ALPHA, chunkPlanes, 1 - ALPHA, 0, chunkPlanes);
        }
        multiplePlanes[i] = chunkPlanes;
    }
    for(const auto &plane : multiplePlanes)
        cv::addWeighted(plane, 1, result, 0, 0, result);

    double time = omp_get_wtime() - start;

    printf("TIME PAR %llu: %f\n", nPlanes, time);
    cv::imwrite("../img/par_" + std::to_string(nPlanes) + ".png", result);
    return time;
}
