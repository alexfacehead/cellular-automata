// RuleSet.h

#ifndef RULESET_H
#define RULESET_H

#include "Grid.h"

class RuleSet
{
public:
    RuleSet();
    ~RuleSet();

    void applyRules(Grid& grid);

private:
    int countNeighbors(const Grid& grid, int x, int y);
    bool applyRule(const Grid& grid, int x, int y);
};

#endif // RULESET_H