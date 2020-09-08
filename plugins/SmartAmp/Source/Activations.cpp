/*
  ==============================================================================

    Activations.cpp
    Created: 11 Jan 2019 11:15:47am
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#include "Activations.h"

namespace {
    typedef float (* activationFunction)(float x);
    void applyActivation(float **data, size_t rows, size_t cols, activationFunction activation)
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
                data[i][j] = activation(data[i][j]);
        }
    }
    
    void applyActivation(float *data, size_t rows, size_t cols, activationFunction activation)
    {
        for (size_t i = 0; i < rows*cols; ++i)
        {
                data[i] = activation(data[i]);
        }
    }
    
    size_t idx(size_t row, size_t col, size_t cols)
    {
        return row * cols + col;
    }
    
    typedef float (* gatedActivationFunction)(float x1, float x2);
    void applyGatedActivation(float *data, size_t rows, size_t cols, gatedActivationFunction activation)
    {
        size_t rowsHalf = rows / 2;
        for (size_t row = 0; row < rowsHalf; ++row)
        {
            size_t startIdx1 = idx(row, 0, cols); 
            size_t startIdx2 = idx(row+rowsHalf, 0, cols);  
            for (size_t col = 0; col < cols; ++col)
                data[startIdx1+col] = activation(data[startIdx1+col], data[startIdx2+col]); 
        }
    }
}

namespace Activations {
    float tanh(float x)
    {
        return tanhf(x);
    }
    
    float sigmoid(float x)
    {
        return 1.0f / (1.0f + expf(-x));
    }
    
    float relu(float x)
    {
        if (x < 0.0f)
            return 0.0f;
        else
            return x;
    }
    
    float softsign(float x)
    {
        return x / (1.0f + fabsf(x));
    }
    
    float linear(float x)
    {
        return x;
    }
    
    float gated(float x1, float x2)  
    {
        return tanh(x1)*sigmoid(x2);
    }
    
    float softgated(float x1, float x2)
    {
        return softsign(x1) * softsign(x2);
    }
    
    void tanh(float** data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)tanh);
    }
    void sigmoid(float** data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)sigmoid);
    }
    void relu(float** data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)relu);
    }
    void softsign(float** data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)softsign);
    }
    void linear(float** data, size_t rows, size_t cols)
    {
        return;
    }
    
    void tanh(float* data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)tanh);
    }
    void sigmoid(float* data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)sigmoid);
    }
    void relu(float* data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)relu);
    }
    void softsign(float* data, size_t rows, size_t cols)
    {
        applyActivation(data, rows, cols, (activationFunction)softsign);
    }
    void linear(float* data, size_t rows, size_t cols)
    {
        return;
    }
    void gated(float* data, size_t rows, size_t cols)
    {
        assert(rows % 2 == 0);
        applyGatedActivation(data, rows, cols, (gatedActivationFunction)gated);
    }
    void softgated(float* data, size_t rows, size_t cols)
    {
        assert(rows % 2 == 0);
        applyGatedActivation(data, rows, cols, (gatedActivationFunction)softgated);
    }
    
    bool isGated(std::string name)
    {
        if ((name == "gated") || (name == "softgated"))
            return true;
        return false;
    }
    
    activationFunction getActivationFunction(std::string name)
    {
        if (name == "tanh")
            return tanh;
        else if (name == "sigmoid")
            return sigmoid;
        else if (name == "relu")
            return relu;
        else if (name == "softsign")
            return softsign;
        else if (name == "linear")
            return linear;
        else
            throw std::invalid_argument("Received unkown activation name.");
    }
    
    activationFuncArray getActivationFuncArray(std::string name)
    {
        if (name == "tanh")
            return tanh;
        else if (name == "sigmoid")
            return sigmoid;
        else if (name == "relu")
            return relu;
        else if (name == "softsign")
            return softsign;
        else if (name == "linear")
            return linear;
        else if (name == "gated")
            return gated;
        else if (name == "softgated")
            return softgated;
        else
            throw std::invalid_argument("Received unkown activation name.");
    }
    
    activationFunc2DArray getActivationFunc2DArray(std::string name)
    {
        if (name == "tanh")
            return tanh;
        else if (name == "sigmoid")
            return sigmoid;
        else if (name == "relu")
            return relu;
        else if (name == "softsign")
            return softsign;
        else if (name == "linear")
            return linear;
        else
            throw std::invalid_argument("Received unkown activation name.");
    }
}
