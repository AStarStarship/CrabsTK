#pragma once
#include <stdafx.h>

#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#include "perceptron.h"

typedef float_t (*PerceptronSensor) ();

#ifndef HEADER_FOR_SENSORY_PERCEPTRON
#define HEADER_FOR_SENSORY_PERCEPTRON

class PerceptronSensor2D {
    public:

    PerceptronSensor2D (uintptr_t width, uintptr_t height);

    uintptr_t GetWidth ();

    uintptr_t GetHeight ();

    Axion_UI1* GetX (uintptr_t width);

    Axion_UI1* GetX (uintptr_t width);

    private:

    uint       width_,
               height_;
    Axion_UI1* x_,
             * y_;
};
#endif  //< #ifndef HEADER_FOR_SENSORY_PERCEPTRON


#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
