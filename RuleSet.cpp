// RuleSet.cpp

#include "RuleSet.h"

RuleSet::RuleSet()
{
    // TODO: Implement constructor (initialize any necessary variables)
}

RuleSet::~RuleSet()
{
    // TODO: Implement destructor (clean up any resources)
}

void RuleSet::applyRules(Grid& grid)
{
    Grid newGrid = grid; // Create a copy of the current grid to store the updated states

    for (int x = 0; x < grid.getWidth(); ++x)
    {
        for (int y = 0; y < grid.getHeight(); ++y)
        {
            bool newState = applyRule(grid, x, y);
            newGrid.setCell(x, y, newState);
        }
    }

    grid = newGrid; // Update the original grid with the new states
}

int RuleSet::countNeighbors(const Grid& grid, int x, int y)
{
    // TODO: Implement countNeighbors (count the number of alive neighbors for the cell at (x, y))
}

bool RuleSet::applyRule(const Grid& grid, int x, int y)
{
    // TODO: Implement applyRule (apply a specific rule based on the cell's state and its neighbors)
}