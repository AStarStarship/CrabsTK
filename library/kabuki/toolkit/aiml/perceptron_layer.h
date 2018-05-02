

#pragma once

#include "perceptron.h"

/** A layer in a Multi-layer Perceptron Network.
    Each Perceptron is individually controlled, but it is convenient to 
*/
class PerceptronLayer {
    public:

    PerceptronLayer (uint32_t neuron_count);

    

    private:

    std::vector<Perceptron> perceptrons;
};