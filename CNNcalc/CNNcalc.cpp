// CNNcalc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "typedefs.h"
// #include "timer.h"
#include "testing_data.h"
#include "cnn.h"

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <process.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>

#include <iostream>
#include <fstream>



//--------------------------------------
// main function
//--------------------------------------
int main()
{
	// Number of test instances
	const int N = 180;

	// Arrays to store test data and expected results
	int64_t inputs[N];
	int   expecteds[N];

	// Timer
	//Timer timer("digitrec software");

	int nbytes;
	int error = 0;
	int num_test_insts = 180;
	int interpreted_digit;


	//--------------------------------------------------------------------
	// Read data from the input file into two arrays
	//--------------------------------------------------------------------
	for (int i = 0; i < N; ++i) {
		int64_t input_digit = testing_data[i];
		// Read expected digit
		int input_value = testing_label[i];

		// Store the digits into arrays
		inputs[i] = input_digit;
		expecteds[i] = input_value;
	}

	//timer.start();


	for (int i = 0; i < N; i++)
	{
		if (expecteds[i] != cnn(inputs[i]))
		{
			error = error + 1;
		}

	}

	//timer.stop();

	// Report overall error out of all testing instances
	std::cout << "Number of test instances = " << num_test_insts << std::endl;
	std::cout << "Overall Error Rate = "
		<< ((double)error / num_test_insts) * 100
		<< "%" << std::endl;


	return 0;
}
