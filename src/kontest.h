#ifndef __KONTEST_H__
#define __KONTEST_H__

#include <libaeds/memory/allocator.h>

#include <probmat.h>


#define KONTEST_MAX 25

Probability* kontest_championship(const Allocator*, ProbMat);


#endif /* __KONTEST_H__ */
