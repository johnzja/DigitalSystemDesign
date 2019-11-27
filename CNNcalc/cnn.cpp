#include "pch.h"
#include "cnn.h"

// 卷积神经网络识别函数
int cnn(int64_t input_data)
{
	float in_c1[49];
	for (int i = 0;i < 49;i++)
	{
		in_c1[i] = float(input_data & 1);
		input_data >>= 1;
	}

	// 第一层卷积层
	float out_c1[32][25];
	for (int i = 0;i < 32;i++)
	{
		float knl[9];
		for (int k = 0;k < 9;k++)
			knl[k] = w_conv1[i * 9 + k];
		conv(in_c1, out_c1[i], knl, 7, 5, 3);	// 卷积
		for (int h = 0;h < 25;h++)
		{
			out_c1[i][h] += b_conv1[i];			// 加上偏置
			if (out_c1[i][h] < 0)
				out_c1[i][h] = 0.0;				// ReLU
		}
	}

	// 第二层卷积层
	float out_c2[64][9];
	for (int i = 0;i < 64;i++)
	{
		for (int h = 0;h < 9;h++)
			out_c2[i][h] = 0;
		float tmp[9];
		for (int j = 0;j < 32;j++)
		{
			float knl[9];
			for (int k = 0;k < 9;k++)
				knl[k] = w_conv2[(i * 32 + j) * 9 + k];
			conv(out_c1[j], tmp, knl, 5, 3, 3);	// 卷积
			for (int h = 0;h < 9;h++)
				out_c2[i][h] += tmp[h];
		}
		for (int h = 0;h < 9;h++)
		{
			out_c2[i][h] += b_conv2[i];			// 加上偏置
			if (out_c2[i][h] < 0)
				out_c2[i][h] = 0.0;				// ReLU
		}
	}

	// 第三层全连接层
	float in_d1[576];
	// Conv2输出的64*9向量需要各矩阵旋转180°后作为Dense1的输入
	for (int c = 0; c < 64; c++)
	{
		for (int h = 0; h < 9; h++)
		{
			int i = c + (8 - h) * 64;
			in_d1[i] = out_c2[c][h];
		}
	}
	float out_d1[256];
	mat_times(out_d1, w_dense1, in_d1, b_dense1, 576, 256);

	// 第四层全连接层
	float out_d2[256];
	mat_times(out_d2, w_dense2, out_d1, b_dense2, 256, 256);

	// 第五层全连接层
	float out_d3[10];
	mat_times(out_d3, w_dense3, out_d2, b_dense3, 256, 10);

	// 判断结果
	float max_prob = 0.0;
	int max_class;
	for (int i = 0;i < 10;i++)
	{
		if (out_d3[i] > max_prob)
		{
			max_prob = out_d3[i];
			max_class = i;
		}
	}

	return max_class;
}

// 卷积操作
int conv(const float * input, float * output, const float * kennel, int i_size, int o_size, int k_size)
{
	if (i_size + 1 != o_size + k_size)
		return -1;

	for (int n = 0;n < o_size*o_size;n++)
	{
		int n1 = n / o_size, n2 = n % o_size;
		output[n] = 0;
		for (int m = 0;m < k_size*k_size;m++)
		{
			int m1 = m / k_size, m2 = m % k_size;
			output[n] += input[(n1 + m1)*i_size + n2 + m2] * kennel[m];
		}
	}
	return 0;
}

// 矩阵乘法y=Ax+b
void mat_times(float * y, const float * A, const float * x, const float * b, int m, int n)
{
	for (int i = 0;i < n;i++)
	{
		y[i] = 0;
		for (int j = 0;j < m;j++)
		{
			y[i] += A[i*m + j] * x[j];	// 乘上权重
		}
		y[i] += b[i];					// 加上偏置
		if (y[i] < 0)
			y[i] = 0.0;					// ReLU
	}
}
