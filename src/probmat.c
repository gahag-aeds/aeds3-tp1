#include "probmat.h"

#include <assert.h>


struct ProbMat {
  const Allocator* allocator;
  
  size_t size;        // The dimension of the matrix.
  Probability data[]; // Flexible member array for the matrix.
};


// Complexity: O(1)
ProbMat* new_probmat(const Allocator* allocator, size_t size) {
  assert(allocator != NULL);
  assert(size > 1);
  
  ProbMat* probmat = al_alloc(
    allocator,
    1,
    sizeof(ProbMat) + sizeof(Probability[size * size])
  );
  
  probmat->allocator = allocator;
  probmat->size = size;
  
  return probmat;
}

// Complexity: O(1)
void delete_probmat(ProbMat** probmat) {
  assert(probmat != NULL);
  
  if (*probmat != NULL)
    al_dealloc((*probmat)->allocator, *probmat);
  
  *probmat = NULL;
}


// Complexity: O(1)
size_t probmat_size(const ProbMat* mat) {
  assert(mat != NULL);
  
  return mat->size;
}

// Complexity: O(1)
Probability* probmat_query(ProbMat* mat, size_t i, size_t j) {
  assert(mat != NULL);
  assert(mat->data != NULL);
  assert(i < mat->size && j < mat->size);
  
  Probability (*data)[mat->size][mat->size] = (void*) mat->data;
  
  return &(*data)[i][j];
}
