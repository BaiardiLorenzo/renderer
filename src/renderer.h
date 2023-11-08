#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "test.h"
#include <omp.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// AoS
struct Circle {
    cv::Scalar color;
    cv::Point center;
    int r;
};

// SoA
struct Circles{
    cv::Scalar* colors;
    cv::Point* centers;
    int* rs;
};


// GENERATION
Circle* generateCircles(std::size_t n, int width, int height, int minRadius, int maxRadius);

Circles* generateSoACircles(std::size_t n);

cv::Mat* generatePlanes(std::size_t nPlanes, Circle circles[], std::size_t nCircles);

// RENDERING
double sequentialRenderer(cv::Mat planes[], std::size_t nPlanes);

double rendererSoASequential(Circles* circles, std::size_t nPlanes, std::size_t nCircles);

double parallelRenderer(cv::Mat planes[], std::size_t nPlanes);

double rendererSoAParallel(Circles* circles, std::size_t nPlanes, std::size_t nCircles);


#endif //RENDERER_RENDERER_H
