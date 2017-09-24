#ifndef __PROBMAT_H__
#define __PROBMAT_H__

#include <stddef.h>


typedef double Probability;

typedef struct ProbMat {
  size_t size;        // The dimension of the matrix.
  Probability* data;  // 2d array for the matrix.
} ProbMat;


ProbMat mk_probmat(Probability*, size_t);

size_t probmat_size(const ProbMat);
Probability* probmat_query(const ProbMat, size_t i, size_t j);


#endif /* __PROBMAT_H__ */
