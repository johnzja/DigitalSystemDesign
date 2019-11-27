#pragma once
#include "pch.h"
#include "typedefs.h"
#include "model_conv.h"
#include "model_dense.h"

// 卷积神经网络识别函数
int cnn(int64_t input_data);

// 卷积操作
int conv(const float * input, float * output, const float * kennel, int i_size, int o_size, int k_size);

// 矩阵乘法
void mat_times(float * y, const float * A, const float * x, const float * b, int m, int n);
