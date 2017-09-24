#ifndef __PROBMAT_H__
#define __PROBMAT_H__

#include <stddef.h>


typedef float Probability; // The probability for a certain event. Range: [0, 1].

typedef struct ProbMat {
  size_t size;        // The dimension of the matrix.
  Probability* data;  // 2d array for the matrix.
} ProbMat;


// Create a ProbMat with the given array.
// The size of the array must be the square of the given size.
// Complexity: O(1)
ProbMat mk_probmat(Probability*, size_t);


// Get the size of the ProbMat.
// Complexity: O(1)
size_t probmat_size(const ProbMat);

// Query a given element in the ProbMat.
// Complexity: O(1)
Probability* probmat_query(const ProbMat, size_t i, size_t j);


#endif /* __PROBMAT_H__ */
