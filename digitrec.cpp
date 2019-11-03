//==========================================================================
//digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor implementation for digit recognition (k=3)

#include "digitrec.h"

//----------------------------------------------------------
// Digitrec
//----------------------------------------------------------
// @param[in] : input - the testing instance
// @return : the recognized digit (0~9)

int digitrec(int64_t input)
{
#include "training_data.h"

	// This array stores K minimum distances per training set
	int knn_set[10][K_CONST];

	// Initialize the knn set
	for (int i = 0; i < 10; i++)
		for (int k = 0; k < K_CONST; k++)
			// Note that the max distance is 49
			knn_set[i][k] = 50;

	for (int i = 0; i < TRAINING_SIZE; i++) {
		for (int j = 0; j < 10; j++) {
			// Read a new instance from the training set
			int64_t training_instance = training_data[j][i];
			// Update the KNN set
			update_knn(input, training_instance, knn_set[j]);
		}
	}

	// Compute the final output
	return knn_vote(knn_set);
}

//-----------------------------------------------------------------------
// update_knn function
//-----------------------------------------------------------------------
// Given the test instance and a (new) training instance, this
// function maintains/updates an array of K minimum
// distances per training set.

// @param[in] : test_inst - the testing instance
// @param[in] : train_inst - the training instance
// @param[in/out] : min_distances[K_CONST] - the array that stores the current
//                  K_CONST minimum distance values per training set

void update_knn(int64_t test_inst, int64_t train_inst, int min_distances[K_CONST])
{
	// Compute the Manhattan distance
	int64_t diff = test_inst ^ train_inst;

	int dist = 0;
	// Count the number of set bits
	for (int i = 0; i < 49; i++) {
		dist += diff & 0x00000001;
		diff = diff >> 1;
	}

	// -----------------------------
	// YOUR CODE GOES HERE

	for (int i = 0;i < 3;i++)
	{
		if (dist < min_distances[i])
		{
			switch(i)
			{
			case 0:
				min_distances[2] = min_distances[1];
				min_distances[1] = min_distances[0];
				min_distances[0] = dist;
			case 1:
				min_distances[2] = min_distances[1];
				min_distances[1] = dist;
			case 2:
				min_distances[2] = dist;
			}
		}
	}
	// -----------------------------

}


//-----------------------------------------------------------------------
// knn_vote function
//-----------------------------------------------------------------------
// Given 10xK minimum distance values, this function 
// finds the actual K nearest neighbors and determines the
// final output based on the most common digit represented by 
// these nearest neighbors (i.e., a vote among KNNs). 
//
// @param[in] : knn_set - 10xK_CONST min distance values
// @return : the recognized digit
// 

int knn_vote(int knn_set[10][K_CONST])
{
	// -----------------------------
	// YOUR CODE GOES HERE

	int mindis;
	int mindis_class;
	int total_mindis_class;
	int mindis_num[10];
	for (int i = 0;i < 10;i++)
	{
		mindis_num[i] = 0;
	}
	for (int n = 0;n < 3;n++)
	{
		mindis = 50;
		mindis_class = 0;
		for (int i = 0;i < 10;i++)
		{
			if (knn_set[i][mindis_num[i]] < mindis)
			{
				mindis = knn_set[i][mindis_num[i]];
				mindis_class = i;
			}
		}
		mindis_num[mindis_class]++;
		if (n == 0)
		{
			total_mindis_class = mindis_class;
		}
	}
	int res;
	for (res = 0;res < 10;res++)
	{
		if (mindis_num[res] > 1)
			break;
	}
	if (res > 9)
	{
		res = total_mindis_class;
	}
	return res;
	// -----------------------------
}

