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
static Group* compute(ProbMat* probmat, Group* groups, const GroupId grp) {
  Group* group = get_group(probmat_size(probmat), groups, grp);
  
  if (group->processed)
    return group;
  
  group->processed = true;
  
  
  uint8_t grp_size = popcount_32(grp);
  
  // For a group of size n, the number of possible picks of two teams is T = (n^2 - n) / 2.
  // Considering all picks are equiprobable, the probability is 1 / T.
  Probability pick = 2.0 / (grp_size * grp_size - grp_size);
  
  // Loop over teams in the group:
  GroupId g1 = grp;
  const GroupId last = setbit_32(0, msb_32(g1));
  for ( // All except last.
    uint8_t first = lsb_32(g1);
    g1 != last;
    g1 = unsetbit_32(g1, first), first = lsb_32(g1)
  ) {
    GroupId g2 = unsetbit_32(g1, first); // all after first.
    for ( // Loop over the next teams:
      uint8_t second = lsb_32(g2);
      g2 != 0;
      g2 = unsetbit_32(g2, second), second = lsb_32(g2)
    ) {
      Group* subgroup_1 = NULL; // subgroup that excludes the second.
      Group* subgroup_2 = NULL; // subgroup that excludes the first.
      Probability prob_1 = pick * (*probmat_query(probmat, first, second))  // 1 beat 2
                , prob_2 = pick * (*probmat_query(probmat, second, first)); // 2 beat 1
      
      if (grp_size > 2) { // There are only subgroups to groups bigger than 2.
        subgroup_1 = compute(probmat, groups, unsetbit_32(grp, second));
        subgroup_2 = compute(probmat, groups, unsetbit_32(grp, first));
      }
      
      
      GroupId g = grp;
      for ( // Loop over the probabilities array of the group:
        uint8_t i = lsb_32(g);
        g != 0;
        g = unsetbit_32(g, i), i = lsb_32(g)
      ) {
        if (second != i) // first wins.
          group->probs[i] += (grp_size < 3) ? prob_1 // no subgroup to group of size 2.
                                            : prob_1 * subgroup_1->probs[i];
        
        if (first != i) // second wins.
          group->probs[i] += (grp_size < 3) ? prob_2 // no subgroup to group of size 2.
                                            : prob_2 * subgroup_2->probs[i];
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
  // Where P(G) is the powerset of G, and U(G) are the unitary subsets of G.
  // The set of possible subgroups therefore has cardinality 2^|G| - |G| - 1.
  // But, excluding those groups causes a huge number of cache misses.
  // Benchmarks have showed slow downs of more than an hour for inputs of size 25.
  // So, lets keep those useless groups anyway.
  const size_t groups_size = 1ul << teams_count;
  
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
