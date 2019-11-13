//==========================================================================
//digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor implementation for digit recognition (k=3)

#include "pch.h"
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
			if (i == 3 && j == 2)
			{
				int a = 0;
			}
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

	int loc;
	for (loc = 0;loc < K_CONST;loc++)
	{
		if (dist < min_distances[loc])
		{
			break;
		}
	}
	for (int i = K_CONST - 1;i > loc;i--)
	{
		min_distances[i] = min_distances[i - 1];
	}
	if (loc < K_CONST)
	{
		min_distances[loc] = dist;
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
	int mindis_num[10];
	for (int i = 0;i < 10;i++)
	{
		mindis_num[i] = 0;
	}
	for (int n = 0;n < K_CONST;n++)
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
	}
	int res = 0;
	int winner;
	for (int i = 0;i < 10;i++)
	{
		if (mindis_num[i] > res)
		{
			res = mindis_num[i];
			winner = i;
		}
	}
	return winner;

	// -----------------------------
}
