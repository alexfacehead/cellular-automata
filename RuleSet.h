// RuleSet.h

#pragma once

#include <vector>
#include <algorithm>
#include "Grid.h"

class RuleSet
{
public:
    RuleSet();
    ~RuleSet();

    /**
     * Applies the rules to the given grid.
     * @param grid The grid to which the rules will be applied.
     */
    void applyRules(Grid& grid);

    /**
     * Sets the custom rules for the RuleSet.
     * @param newRules A vector of integers representing the custom rules.
     */
    void setCustomRules(const std::vector<int>& newRules);

private:
    std::vector<int> ruleNumbers;

    /**
     * Counts the number of alive neighbors for the cell at (x, y) in the grid.
     * @param grid The grid containing the cells.
     * @param x The x-coordinate of the cell.
     * @param y The y-coordinate of the cell.
     * @return The number of alive neighbors.
     */
    int countNeighbors(const Grid& grid, int x, int y);

    /**
     * Applies the rule based on the cell's state and its neighbors.
     * @param grid The grid containing the cells.
     * @param x The x-coordinate of the cell.
     * @param y The y-coordinate of the cell.
     * @return The new state of the cell after applying the rule.
     */
    bool applyRule(const Grid& grid, int x, int y);
};