#include <stdio.h>
#include <stdlib.h>

#include <libaeds/data/resources/resource.h>
#include <libaeds/memory/allocator.h>

#include <probmat.h>


int main() {
  Allocator allocator = std_allocator(abort);
  Resources res = new_resources(&allocator);
  
  unsigned short size;
  
  if (scanf("%hud", &size) != 1)
    return delete_resources(&res), -1;
  
  ProbMat* matrix = new_probmat(&allocator, size);
  rs_register(
    &matrix,
    rs_disposer((void (*)(void*)) delete_probmat),
    &res
  );
  
  
  for (unsigned short i = 0; i < size; i++)
    for (unsigned short j = 0; j < size; j++)
      if (scanf("%f", probmat_query(matrix, i, j)) != 1)
          return delete_resources(&res), -1;
  
  
  return delete_resources(&res), 0;
}
