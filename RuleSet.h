#ifndef RULESET_H
#define RULESET_H

#include "Cell.h"

class Grid; // Add this forward declaration

class RuleSet
{
public:
    RuleSet();
    ~RuleSet();

    int countNeighbors(const Grid& grid, int x, int y) const;
    bool applyRuleConway(const Grid& grid, int x, int y) const;
};

#endif