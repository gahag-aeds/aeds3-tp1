#ifndef __KONTEST_H__
#define __KONTEST_H__

#include <libaeds/memory/allocator.h>

#include <probmat.h>


#define KONTEST_MAX 25  // The maximum players to compete
                        // in the mortal kontest championship.


// For a given ProbMat, calculate the probabilities
// of each player winning the correspondent mortal kontest championship.
// The caller has the responsibility to free the returned pointer with the given allocator.
// Complexity: O(n^2 * 2^n)
Probability* kontest_championship(const Allocator*, ProbMat*);


#endif /* __KONTEST_H__ */
