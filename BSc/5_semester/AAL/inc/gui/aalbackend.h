#ifndef AALBACKEND_H
#define AALBACKEND_H

#include <QObject>

#include "inc/algorithm/alg_params.hpp"

class AALBackend :public QObject {
    Q_OBJECT
public:
    AALBackend();

    void computeBFS(AlgorithmParams, AdditionalBFSParams);
    void computeNaive(AlgorithmParams, AdditionalNaiveParams);
    void computeAstar(AlgorithmParams, AdditionalAstarParams);

    static std::string generateRandom(std::size_t len);
    static std::string generateRandomPermutation(RandomPermutationParams );
    static std::string generateRandomWithChance(std::size_t len, double chance);

signals:
    void updateStatusBar(std::string );
    void computationFinished(AlgorithmResult );   
};

#endif // AALBACKEND_H
