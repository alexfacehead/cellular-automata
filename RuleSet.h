#ifndef RULESET_H
#define RULESET_H

#include "Cell.h"
#include <string>
#include <vector>

class Grid;

class RuleSet
{
public:
    RuleSet();
    ~RuleSet();

    std::string name;
    std::string category;
    bool birthConditions[9];    // index = neighbor count (0-8)
    bool survivalConditions[9];

    void setRule(const std::string& ruleName,
                 const std::vector<int>& birth,
                 const std::vector<int>& survival);
    int countNeighbors(const Grid& grid, int x, int y) const;
    bool applyRuleChosen(const Grid& grid, int x, int y) const;
    std::string getNotation() const;

    static std::vector<RuleSet> getPresets();
};

#endif
