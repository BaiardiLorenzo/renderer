#include <iostream>
#include "renderer.h"
#include <vector>

#define MAX_TESTS 100
#define MIN_TEST 10
#define N_CIRCLES 100

int main() {
#ifdef _OPENMP
    printf("Core/Threads: %d\n", omp_get_num_procs());
    omp_set_dynamic(0);
#endif
    std::vector<double> sequentialTimes;
    std::vector<double> parallelTimes;
    std::vector<int> testPlanes;
    for (int i = MIN_TEST; i <= MAX_TESTS; i += 10)
        testPlanes.push_back(i);

    for (int i = 2; i < omp_get_num_procs(); i++) {
        omp_set_num_threads(i);
        printf("TEST with Core/Threads: %d\n", i);
        for (int test: testPlanes) {
            const unsigned long long n = test * N_CIRCLES;
            Circle circles[n];
            generateCircles(circles, n);
            double t1 = rendererSequential(circles, test, N_CIRCLES);
            double t2 = rendererParallel(circles, test, N_CIRCLES);
            printf("SPEEDUP TEST %d: %f \n", test, t1 / t2);
            sequentialTimes.push_back(t1);
            parallelTimes.push_back(t2);
            printf("\n");
        }
    }
}

