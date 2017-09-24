#include "kontest.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <libaeds/data/bits.h>


typedef uint32_t GroupId; // The KONTEST_MAX is 25, so 32 bits is enough.

typedef struct Group {
  bool processed;
  Probability probs[KONTEST_MAX];
} Group;


static Group* compute(ProbMat probmat, Group* groups, GroupId grp) {
  Group* group = &groups[grp];
  
  if (group->processed)
    return group;
  
  group->processed = true;
  
  
  uint8_t grp_size = popcount_32(grp);
  
  Probability pick = 2.0 / (grp_size * grp_size - grp_size);
  
  for (uint8_t first = 0; first < probmat.size - 1; first++) {
    if (!(grp & (1 << first)))   // the bit correspondent to the first is not set
      continue;
    
    for (uint8_t second = first + 1; second < probmat.size; second++) {
      if (!(grp & (1 << second)))   // the bit correspondent to the second is not set
        continue;
      
      for (uint8_t i = 0; i < probmat.size; i++) {
        // second loses:
        if (second != i) {
          // subgroup that excludes the second:
          Group* subgroup = compute(probmat, groups, grp & ~(1 << second));
          
          Probability prob = pick * (*probmat_query(probmat, first, second));
          
          if (grp_size > 2)
            prob *= subgroup->probs[i];

          group->probs[i] += prob;
        }
        
        // first loses:
        if (first != i) {
          // subgroup that excludes the first:
          Group* subgroup = compute(probmat, groups, grp & ~(1 << first));
          
          Probability prob = pick * (*probmat_query(probmat, second, first));
          
          if (grp_size > 2)
            prob *= subgroup->probs[i];
          
          group->probs[i] += prob;
        }
      }
    }
  }
  
  return group;
}


Probability* kontest_championship(const Allocator* allocator, ProbMat probmat) {
  // All possible subgroups of a group G are: P(G) \ U(G) \ {}
  // Where P(G) is the powerset of G, U(G) are the unitary subsets of G.
  // The set of possible subgroups would therefore have cardinality 2^|G| - |G| - 1.
  // But, to use the GroupId as the indexer for the array, the useless unitary
  // groups, as well as the empty group, are kept.
  const size_t groups_size = 1ul << probmat.size; // 2^probmat.size
  
  Group* groups = al_alloc_clear(
    allocator,
    groups_size,
    sizeof(Group) // + sizeof(Probability[probmat.size])
  );
  
  
  Group* group = compute(probmat, groups, groups_size - 1); // The last group
                                                            // is the total group.
  
  Probability* result = al_alloc(allocator, 1, sizeof(Probability[probmat.size]));
  
  memcpy(result, group->probs, sizeof(Probability[probmat.size]));
  
  
  al_dealloc(allocator, groups);
  
  
  return result;
}
