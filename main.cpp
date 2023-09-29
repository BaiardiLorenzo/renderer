#include "renderer.h"

void headerResults(){
    std::ofstream outfile;
    outfile.open(TEST_PATH);
    if(outfile.is_open())
        outfile << HEADER_TEST;
    outfile.close();
}

void exportResults(const std::string& type, int nThreads, double speedUp, std::size_t test, double tSeq, double tPar){
    std::ofstream outfile;
    outfile.open(TEST_PATH, std::ios::out | std::ios::app);
    if (outfile.is_open())
        outfile<<type<<";"<<nThreads<<";"<<speedUp<<";"<<test<<";"<<tSeq<<";"<<tPar<<"\n";
    outfile.close();
}

int main() {
#ifdef _OPENMP
    printf("**Number of cores/threads: %d**\n", omp_get_num_procs());
    omp_set_dynamic(0);
#endif
    headerResults();
    std::vector<std::size_t> testPlanes;
    for (std::size_t i = MIN_TEST; i <= MAX_TESTS; i += SPACE)
        testPlanes.push_back(i);

    for (int i=2; i<=omp_get_num_procs(); i*=2) {
        //SET NUMBER OF THREADS
        printf("Number of cores/threads used: %d\n", i);
        omp_set_num_threads(i); // SET NUMBER OF THREADS
        for (auto test: testPlanes) {
            printf("TEST AoS: %llu\n", test);
            // GENERATION OF CIRCLES
            std::size_t n = test * N_CIRCLES;
            auto circles = generateCircles(n);

            // TEST SEQUENTIAL AND PARALLEL
            double tSeq = rendererSequential(circles, test, N_CIRCLES);
            double tPar = rendererParallel(circles, test, N_CIRCLES);

            double speedUp = tSeq / tPar;
            printf("Speedup: %f \n\n", speedUp);

            // WRITE RESULTS TO TXT FILE
            exportResults("AoS",i,speedUp,test,tSeq,tPar);

            // DELETE ARRAY DYNAMIC ALLOCATED
            delete[] circles;

            printf("TEST SoA: %llu\n", test);
            // GENERATION OF CIRCLES
            auto circlesSoA = generateSoACircles(n);

            // TEST SEQUENTIAL AND PARALLEL
            tSeq = rendererSoASequential(circlesSoA, test, N_CIRCLES);
            tPar = rendererSoAParallel(circlesSoA, test, N_CIRCLES);

            speedUp = tSeq / tPar;
            printf("Speedup: %f \n\n", speedUp);

            // WRITE RESULTS TO TXT FILE
            exportResults("SoA",i,speedUp,test,tSeq,tPar);

            // DELETE ARRAY DYNAMIC ALLOCATED
            delete[] circlesSoA->colors;
            delete[] circlesSoA->centers;
            delete[] circlesSoA->rs;
            delete[] circlesSoA;
        }
    }
    return 0;
}

