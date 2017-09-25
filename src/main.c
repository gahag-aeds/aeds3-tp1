#include <stdio.h>
#include <stdlib.h>

#include <libaeds/memory/allocator.h>
#include <libaeds/data/resources/memory.h>
#include <libaeds/data/resources/resource.h>

#include <probmat.h>
#include <kontest.h>


int main() {
  const Allocator allocator = std_allocator(abort);
  Resources res = new_resources(&allocator);
  
  
  unsigned short size;
  
  if (scanf("%hud", &size) != 1)
    return delete_resources(&res), -1;
  
  
  ProbMat* probmat = new_probmat(&allocator, size);
  rs_register(
    &probmat,
    rs_disposer((void (*)(void*)) delete_probmat),
    &res
  );
  
  for (unsigned short i = 0; i < size; i++)
    for (unsigned short j = 0; j < size; j++)
      if (scanf("%f", probmat_query(probmat, i, j)) != 1)
          return delete_resources(&res), -1;
  
  
  Probability* results = kontest_championship(&allocator, probmat);
  rs_register(
    results,
    rs_disposer_d((void*) &allocator, al_dealloc_void),
    &res
  );
  
  for (unsigned short i = 0; i < size; i++)
    if (printf("%.4f ", results[i]) < 0)
      return delete_resources(&res), -1;
  
  if (puts("") < 0)
    return delete_resources(&res), -1;
  
  
  return delete_resources(&res), 0;
}
