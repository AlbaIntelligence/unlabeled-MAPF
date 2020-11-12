#include "../include/goal_allocator.hpp"

GoalAllocator::GoalAllocator(Problem* _P)
  : P(_P)
{
}

GoalAllocator::~GoalAllocator()
{
}

void GoalAllocator::assign()
{
  LibGA::OpenList OPEN(LibGA::FieldEdge::compare);

  // setup open list
  LibGA::FieldEdges GC_Edge;
  for (int i = 0; i < P->getNum(); ++i) {
    auto s = P->getStart(i);
    for (int j = 0; j < P->getNum(); ++j) {
      auto g = P->getGoal(j);
      auto p = new LibGA::FieldEdge(i, j, s, g, s->manhattanDist(g));
      GC_Edge.push_back(p);
      OPEN.push(p);
    }
  }

  auto matching = LibGA::Matching(P);

  while (!OPEN.empty()) {
    auto p = OPEN.top();
    OPEN.pop();

    // lazy evaluation
    if (!p->evaled) {
      p->setRealDist(P->getG()->pathDist(p->s, p->g));
      OPEN.push(p);
      continue;
    }

    matching.update(p);
    if (matching.matched_num == P->getNum()) break;
  }

  assigned_goals = matching.assigned_goals;

  // memory management
  for (auto p : GC_Edge) delete p;
}

Nodes GoalAllocator::getAssignedGoals() const
{
  return assigned_goals;
}