#include "kontest.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <libaeds/data/bits.h>


// A GroupId identifies a group. Each team in the championship is represented by one bit
// in the GroupId. If the bit is set, the team is in the group. Otherwise, the team has
// already been eliminated.
// The KONTEST_MAX is 25, so 32 bits is enough.
typedef uint32_t GroupId; 

typedef struct Group {
  bool processed; // Whether the probabilities for the group have already been calculated.
  
  // The probabilities of each team winning in this group.
  // The indexing in this array is correspondent to the bit index in the GroupId.
  // Therefore, is a team is represented by the 3rd bit in the GroupId, it's winning
  // probability will be probs[3].
  Probability probs[];
} Group;


// Indexing for a group array.
// As the probs member of the Group struct is a flexible member array,
// standard bracket indexing does not work correctly.
// Complexity: O(1)
static Group* get_group(size_t probs_size, Group* groups, GroupId id) {
  uint8_t* const groups_bytes = (void*) groups;
  
  const size_t group_size = sizeof(Group) + sizeof(Probability[probs_size]);
  
  return (void*) (groups_bytes + id * group_size);
}


// Function to compute the probabilities for a specific group.
// If the probabilities have already been calculated, no recalculations are made.
// Complexity: O()
static Group* compute(ProbMat* probmat, Group* groups, GroupId grp) {
  const size_t teams_count = probmat_size(probmat);
  
  // Due to the exclusion of the empty and unitary groups,
  // it is necessary to exclude them from the indexing.
  // ceil(log2(grp)) is the number of unitary groups that precede the given group.
  // 1 corresponds to the empty group.
  const size_t group_ix = grp - (size_t) ceil(log2(grp)) - 1;
  
  Group* group = get_group(teams_count, groups, group_ix);
  
  if (group->processed)
    return group;
  
  group->processed = true;
  
  
  uint8_t grp_size = popcount_32(grp);
  
  // For a group of size n, the number of possible picks of two teams is
  // T = (n^2 - n) / 2
  // Considering all picks are equiprobable, the probability is
  // 1 / T
  Probability pick = 2.0 / (grp_size * grp_size - grp_size);
  
  // Loop over teams in the group:
  for (uint8_t first = 0; first < teams_count - 1; first++) {
    if (!testbit_32(grp, first))  // Team is not in the group.
      continue;
    
    // Loop over the next teams:
    for (uint8_t second = first + 1; second < teams_count; second++) {
      if (!testbit_32(grp, second)) // Team is not in the group.
        continue;
      
      // Loop over the probabilities array of the group:
      for (uint8_t i = 0; i < teams_count; i++) {
        // second loses:
        if (second != i) {
          Probability prob = pick * (*probmat_query(probmat, first, second));
          
          if (grp_size > 2) { // There are only subgroups to groups bigger than 2.
            // subgroup that excludes the second:
            Group* subgroup = compute(probmat, groups, unsetbit_32(grp, second));
            
            prob *= subgroup->probs[i];
          }

          group->probs[i] += prob;
        }
        
        // first loses:
        if (first != i) {
          Probability prob = pick * (*probmat_query(probmat, second, first));
          
          if (grp_size > 2) { // There are only subgroups to groups bigger than 2.
            // subgroup that excludes the first:
            Group* subgroup = compute(probmat, groups, unsetbit_32(grp, first));

            prob *= subgroup->probs[i];
          }
          
          group->probs[i] += prob;
        }
      }
    }
  }
  
  return group;
}


// Complexity: O()
Probability* kontest_championship(const Allocator* allocator, ProbMat* probmat) {
  assert(allocator != NULL);
  assert(probmat != NULL);
  assert(probmat_size(probmat) > 1);
  
  const size_t teams_count = probmat_size(probmat);
  
  // All possible subgroups of a group G are: P(G) \ U(G) \ {}
  // Where P(G) is the powerset of G, U(G) are the unitary subsets of G.
  // The set of possible subgroups therefore has cardinality 2^|G| - |G| - 1.
  const size_t groups_size = (1ul << teams_count) - teams_count - 1;
  
  Group* groups = al_alloc_clear(
    allocator,
    groups_size,
    sizeof(Group) + sizeof(Probability[teams_count])
  );
  
  
  const GroupId base_groupid = (1ul << teams_count) - 1; // Group with all teams.
  
  Group* group = compute(probmat, groups, base_groupid); 
  
  
  Probability* result = al_alloc(allocator, 1, sizeof(Probability[teams_count]));
  
  memcpy(result, group->probs, sizeof(Probability[teams_count]));
  
  
  al_dealloc(allocator, groups);
  
  
  return result;
}
