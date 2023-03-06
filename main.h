#ifndef RENDERER_MAIN_H
#define RENDERER_MAIN_H

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

struct Circle{
    cv::Point center;
    cv::Scalar color;
    int r;
};

Circle * generate_circles(int);

double renderer_seq(int, int);
double renderer_circles_par(int, int);
double renderer_planes_circles_par(int, int);

#endif //RENDERER_MAIN_H
