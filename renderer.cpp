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
    printf("RENDERER SEQUENTIAL %llu: ", nPlanes);
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    cv::Mat planes[nPlanes];

    //START
    double start = omp_get_wtime();

    for (int i = 0; i < nPlanes; i++) {
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i*nCircles+j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }

    for (auto &plane: planes)
        combinePlanesSequential(&result, &plane);

    double time = omp_get_wtime() - start;
    //END

    printf(" TIME %f sec.\n", time);

    cv::imwrite("../img/seq_" + std::to_string(nPlanes) + ".png", result);
    return time;
}

double rendererParallel(Circle circles[], unsigned long long nPlanes, unsigned long long nCircles) {
    printf("RENDERER PARALLEL %llu: ", nPlanes);
    cv::Mat result(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
    cv::Mat planes[nPlanes];

    double start = omp_get_wtime();

#pragma omp parallel for default(none) shared(planes, circles, result) firstprivate(nPlanes, nCircles)
    for (int i = 0; i < nPlanes; i++) {
        planes[i] = cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT);
        for (int j = 0; j < nCircles; j++) {
            Circle circle = circles[i*nCircles+j];
            cv::circle(planes[i], circle.center, circle.r, circle.color, cv::FILLED, cv::LINE_AA);
        }
    }

    combinePlanesParallel(&result, planes, nPlanes);

    double time = omp_get_wtime() - start;

    printf(" TIME %f sec.\n", time);

    cv::imwrite("../img/par_" + std::to_string(nPlanes) + ".png", result);
    return time;
}

void combinePlanesSequential(cv::Mat* src1, cv::Mat* src2){
    int cn = src1->channels();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for (int c = 0; c < cn - 1; c++) {
                unsigned char src1Px = src1->data[i * src1->step + j * cn + c];
                unsigned char src2Px = src2->data[i * src2->step + j * cn + c];
                src1->data[i * src1->step + cn * j + c] = ((src2Px * (1-ALPHA)*255) + (src1Px * ALPHA * 255))/255;
            }
            double alphaSrc1 = (double) (src1->data[i * src1->step + j * cn + 3]) / 255;
            double alphaSrc2 = (double) (src2->data[i * src2->step + j * cn + 3]) / 255;
            src1->data[i * src1->step + cn * j + 3] = ((alphaSrc1 * ALPHA) + (alphaSrc2 * (1-ALPHA))) * 255;
        }
    }
}

void combinePlanesParallel(cv::Mat* result, cv::Mat planes[], unsigned long long nPlanes){
    int cn = result->channels();
#pragma omp parallel for default(none) shared(result, planes) firstprivate(nPlanes, cn)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for(int z = 0; z < nPlanes; z++) {
                cv::Mat* src2 = &planes[z];
                for (int c = 0; c < cn - 1; c++) {
                    unsigned char src1Px = result->data[i * result->step + j * cn + c];
                    unsigned char src2Px = src2->data[i * src2->step + j * cn + c];
                    result->data[i * result->step + cn * j + c] =
                            ((src2Px * (1 - ALPHA) * 255) + (src1Px * ALPHA * 255)) / 255;
                }
                double alphaSrc1 = (double) (result->data[i * result->step + j * cn + 3]) / 255;
                double alphaSrc2 = (double) (src2->data[i * src2->step + j * cn + 3]) / 255;
                result->data[i * result->step + cn * j + 3] = ((alphaSrc1 * ALPHA) + (alphaSrc2 * (1 - ALPHA)))*255;
            }
        }
    }

}

/*
void combinePlanesSequential(cv::Mat* src1, cv::Mat* src2){
    if(src1->channels() == src2->channels()) {
        int cn = src1->channels();
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                double alphaSrc = (double) (src1->data[i * src1->step + j * cn + 3]) / 255;
                double alphaOverlay = (double) (src2->data[i * src2->step + j * cn + 3]) / 255;
                double alpha = alphaOverlay + alphaSrc * (1 - alphaOverlay);
                for (int c = 0; c < cn - 1; c++) {
                    unsigned char overlayPx;
                    unsigned char srcPx;
                    overlayPx = src2->data[i * src2->step + j * cn + c];
                    srcPx = src1->data[i * src1->step + j * cn + c];
                    src1->data[i * src1->step + cn * j + c] =
                            (overlayPx * alphaOverlay + srcPx * alphaSrc * (1 - alphaOverlay)) / alpha;
                }
                src1->data[i * src1->step + cn * j + 3] = alpha * 255;
            }
        }
    }
}
*/