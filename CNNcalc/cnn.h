#pragma once

#include "typedefs.h"
#include "model_conv.h"
#include "model_dense.h"

// ����������ʶ����
int cnn(int64_t input_data);

// ��������
int conv(const float * input, float * output, const float * kennel, int i_size, int o_size, int k_size);

// ����˷�
void mat_times(float * y, const float * A, const float * x, const float * b, int m, int n);
