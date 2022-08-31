/*
  ==============================================================================

    Activations.h
    Created: 11 Jan 2019 11:15:47am
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#pragma once
#include <assert.h>
#include <cmath>
#include <string>
#include <stdexcept>

namespace Activations {
    float tanh(float x);
    float sigmoid(float x);
    float relu(float x);
    float softsign(float x);
    float linear(float x);
    float gated(float x1, float x2);
    float softgated(float x1, float x2);
    
    void tanh(float** data, size_t rows, size_t cols);
    void sigmoid(float** data, size_t rows, size_t cols);
    void relu(float** data, size_t rows, size_t cols);
    void softsign(float** data, size_t rows, size_t cols);
    void linear(float** data, size_t rows, size_t cols);
    void gated(float** data, size_t rows, size_t cols);
    
    void tanh(float* data, size_t rows, size_t cols);
    void sigmoid(float* data, size_t rows, size_t cols);
    void relu(float* data, size_t rows, size_t cols);
    void softsign(float* data, size_t rows, size_t cols);
    void linear(float* data, size_t rows, size_t cols);
    void gated(float* data, size_t rows, size_t cols);
    void softgated(float* data, size_t rows, size_t cols);
    
    typedef float (* activationFunction)(float x);
    typedef void (* activationFuncArray)(float *x , size_t rows, size_t cols);
    typedef void (* activationFunc2DArray)(float **x , size_t rows, size_t cols);
    
    bool isGated(std::string name);
    activationFunction getActivationFunction(std::string name);
    activationFuncArray getActivationFuncArray(std::string name);
    activationFunc2DArray getActivationFunc2DArray(std::string name);
}
