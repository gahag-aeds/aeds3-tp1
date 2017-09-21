#include "probmat.h"

#include <assert.h>


struct ProbMat {
  const Allocator* allocator;
  
  size_t size;  // The dimension of the matrix.
  float data[]; // 2d flexible member array for the matrix.
};


ProbMat* new_probmat(const Allocator* allocator, size_t size) {
  assert(allocator != NULL);
  assert(size > 0);
  
  ProbMat* mat = al_alloc(
    allocator,
    1,
    sizeof(ProbMat) + sizeof(float[size][size])
  );
  
  mat->allocator = allocator;
  mat->size = size;
  
  return mat;
}

void delete_probmat(ProbMat** _mat) {
  assert(_mat != NULL);
  
  ProbMat* mat = *_mat;
  
  if (mat != NULL) {
    al_dealloc(mat->allocator, mat);
    *_mat = NULL;
  }
}


size_t probmat_size(const ProbMat* mat) {
  assert(mat != NULL);
  
  return mat->size;
}

float* probmat_query(const ProbMat* mat, size_t i, size_t j) {
  assert(mat != NULL);
  assert(i < mat->size && j < mat->size);
  
  float (*data)[mat->size][mat->size] = (void*) &(mat->data);
  
  return &(*data)[i][j];
}
