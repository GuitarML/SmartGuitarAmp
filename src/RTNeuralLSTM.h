#pragma once

#include <RTNeural/RTNeural.h>
#include <nlohmann/json.hpp>

class RT_LSTM
{
public:
    RT_LSTM() = default;

    void reset();
    //void load_json3(const char* jsonFile);
    void load_json3(const nlohmann::json& weights_json);

    //void process(const float* inData, float* outData, int numSamples);
    void process(const float* inData, float param1, float param2, float* outData, int numSamples);

    int input_size = 3;

    float previousParam1 = 0.0;
    float previousParam2 = 0.0;
    float steppedValue1 = 0.0;
    float steppedValue2 = 0.0;
    bool changedParam1 = false;
    bool changedParam2 = false;

private:
    RTNeural::ModelT<float, 3, 1,
        RTNeural::LSTMLayerT<float, 3, 20>,
        RTNeural::DenseT<float, 20, 1>> model_cond2;

    float inArray alignas(16)[3] = { 0.0, 0.0, 0.0 };
    //float inArray[2] = { 0.0, 0.0, 0.0 };
};
