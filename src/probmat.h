#ifndef __PROBMAT_H__
#define __PROBMAT_H__

#include <libaeds/memory/allocator.h>


typedef struct ProbMat ProbMat;


ProbMat* new_probmat(const Allocator*, size_t);
void delete_probmat(ProbMat**);


size_t probmat_size(const ProbMat*);
float* probmat_query(const ProbMat*, size_t i, size_t j);

#endif /* __PROBMAT_H__ */
