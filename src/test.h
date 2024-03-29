#ifndef RENDERER_TEST_H
#define RENDERER_TEST_H

// PATH RESULT FILE
#define RESULT_PATH ("../results/csv/result-"+std::to_string(WIDTH)+"-"+std::to_string(HEIGHT)+".csv")
#define RESULT_CIRCLES_PATH ("../results/csv/result-"+std::to_string(WIDTH)+"-"+std::to_string(HEIGHT)+"-circles.csv")
#define SEQ_IMG_PATH "../results/img/seq/"
#define PAR_IMG_PATH "../results/img/par/"

// IMAGES
#define WIDTH 1024
#define HEIGHT 1024
#define TRANSPARENT cv::Scalar(255, 255, 255, 0)
#define TRANSPARENT_MAT cv::Mat(HEIGHT, WIDTH, CV_8UC4, TRANSPARENT)
#define ALPHA 0.5

// RADIUS CIRCLES
#define MAX_RADIUS 200
#define MIN_RADIUS 10

// FOR TESTING
#define MAX_TESTS 10000
#define MIN_TEST 1000
#define SPACE 1000
#define N_CIRCLES 50


#endif //RENDERER_TEST_H
