#ifndef __PROBMAT_H__
#define __PROBMAT_H__

#include <stddef.h>

#include <libaeds/memory/allocator.h>


typedef float Probability; // The probability for a certain event. Range: [0, 1].

typedef struct ProbMat ProbMat; // A matrix of probabilities.


// Create a ProbMat of the given size.
// Complexity: O(1)
ProbMat* new_probmat(const Allocator*, size_t);

// Delete a ProbMat created with new_probmat.
// Complexity: O(1)
void delete_probmat(ProbMat**);


// Get the size of the ProbMat.
// Complexity: O(1)
size_t probmat_size(const ProbMat*);

// Query a given element in the ProbMat.
// Complexity: O(1)
Probability* probmat_query(ProbMat*, size_t i, size_t j);


#endif /* __PROBMAT_H__ */
