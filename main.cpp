#include <iostream>
#include "renderer.h"
#include <fstream>
#include <vector>

#define MAX_TESTS 100
#define MIN_TEST 10
#define N_CIRCLES 100

int main() {
#ifdef _OPENMP
    printf("Core/Threads: %d\n", omp_get_num_procs());
    omp_set_dynamic(0);
#endif
    std::ofstream outfile;
    std::string fileName = "../test.csv";
    outfile.open(fileName, std::ios::out | std::ios::app);
    if(outfile.is_open())
        outfile << "THREADS ; SPEEDUP ; TEST ; SEQ ; PAR \n";
    outfile.close();
    std::vector<double> sequentialTimes;
    std::vector<double> parallelTimes;
    std::vector<int> testPlanes;
    for (int i = MIN_TEST; i <= MAX_TESTS; i += 10)
        testPlanes.push_back(i);

    for (int i = 2; i <= omp_get_num_procs(); i *=2) {
        //SET NUMBER OF THREADS
        omp_set_num_threads(i);
        printf("TEST with Core/Threads: %d\n", i);
        for (int test: testPlanes) {
            // GENERATION OF CIRCLES
            const unsigned long long n = test * N_CIRCLES;
            Circle circles[n];
            generateCircles(circles, n);

            // TEST SEQUENTIAL AND PARALLEL
            double t1 = rendererSequential(circles, test, N_CIRCLES);
            double t2 = rendererParallel(circles, test, N_CIRCLES);
            double speedUp = t1/t2;

            printf("SPEEDUP TEST %d: %f \n\n", test, speedUp);
            sequentialTimes.push_back(t1);
            parallelTimes.push_back(t2);

            // WRITE TO TEXT
            outfile.open(fileName, std::ios::out | std::ios::app);
            if(outfile.is_open())
                outfile<<i<<";"<<speedUp<<";"<<test<<";"<<t1<<";"<<t2<<"\n";
            outfile.close();
        }
    }
    return 0;
}

