#ifndef RULESET_H
#define RULESET_H

#include "Cell.h"

class Grid; // Add this forward declaration

class RuleSet
{
public:
    // Constructor and destructor
    RuleSet();
    ~RuleSet();

    // Methods
    int countNeighbors(const Grid& grid, int x, int y) const;
    bool applyRuleConway(const Grid& grid, int x, int y) const;
    bool applyRuleChosen(const Grid& grid, int x, int y) const; // Add this line
};

#endif