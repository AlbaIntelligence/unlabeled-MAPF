/*
 * Implementation of ICBS for Iterative Refinement
 * Do not use this directly
 */

#pragma once
#include "cbs_refine.hpp"
#include "icbs.hpp"

class ICBS_REFINE : public ICBS, CBS_REFINE
{
private:
  void setInitialHighLevelNode(HighLevelNode_p n);
  Path getConstrainedPath(HighLevelNode_p h_node, int id);
  LibCBS::Constraints getPrioritizedConflict(HighLevelNode_p h_node);

public:
  ICBS_REFINE(Problem* _P, const Plan& _old_plan,
              const std::vector<int>& _modif_list);
  ~ICBS_REFINE(){};
};
