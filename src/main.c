#include <stdio.h>
#include <stdlib.h>

#include <libaeds/memory/allocator.h>

#include <probmat.h>
#include <kontest.h>



Probability probdata[KONTEST_MAX * KONTEST_MAX];

int main() {
  const Allocator allocator = std_allocator(abort);
  
  
  unsigned short size;
  
  if (scanf("%hud", &size) != 1)
    return -1;
  
  
  ProbMat probmat = mk_probmat(probdata, size);
  
  for (unsigned short i = 0; i < size; i++)
    for (unsigned short j = 0; j < size; j++)
      if (scanf("%lf", probmat_query(probmat, i, j)) != 1)
          return -1;
  
  
  Probability* results = kontest_championship(&allocator, probmat);
  
  for (unsigned short i = 0; i < size; i++)
    if (printf("%lf ", results[i]) < 0)
      return -1;
  
  al_dealloc(&allocator, results);
  
  
  return 0;
}
