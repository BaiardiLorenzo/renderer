#include <fstream>
#include "utils.h"

void headerResults(const std::string& filename, int nThreads){
    std::ofstream outfile;
    outfile.open(filename);
    if(outfile.is_open())
        outfile << "TEST;T_SEQ;";
    for(int i=2; i<=nThreads; i+=2)
        outfile << "T_PAR" << i << ";SPEEDUP" << i << ";";
    outfile.close();
}

void exportResults(const std::string& filename, std::size_t test, double tSeq, const std::map<std::size_t, double>& tPars,
                   std::map<std::size_t,double> speedUps){
    std::ofstream outfile;
    outfile.open(filename, std::ios::out | std::ios::app);
    if(outfile.is_open()){
        outfile << std::fixed << std::setprecision(3);
        outfile << test << ";" << tSeq << ";";
        for(auto tPar: tPars)
            outfile << tPar.second << ";" << speedUps[tPar.first] << ";";
    }
    outfile.close();
}

void headerResultsCircle(const std::string& filename){
    std::ofstream outfile;
    outfile.open(filename);
    if(outfile.is_open())
        outfile << "TEST_P;TEST_C;T_SEQ;T_PAR;SPEEDUP\n";
    outfile.close();
}

void exportResultsCircle(const std::string& filename, std::size_t testP, std::size_t testC, double tSeq, double tPar, double speedUp){
    std::ofstream outfile;
    outfile.open(filename, std::ios::out | std::ios::app);
    if(outfile.is_open()){
        outfile << std::fixed << std::setprecision(3);
        outfile << testP << ";" << testC << ";" << tSeq << ";" << tPar << ";" << speedUp << "\n";
    }
    outfile.close();
}
