//===========================================================================
// testing_data.h
//===========================================================================
// @brief: This header defines a testing set containing 180 data and 
//         corresponding labels.

#ifndef TESTING_DATA_H
#define TESTING_DATA_H

#include "typedefs.h"

#define TESTING_SIZE 180

const int64_t testing_data[TESTING_SIZE] = {
    #include "data/testing_data.dat"
};

const int testing_label[TESTING_SIZE] = {
    #include "data/testing_label.dat"
};

#endif