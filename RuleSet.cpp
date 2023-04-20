// RuleSet.cpp

#include "RuleSet.h"
#include "Grid.h"

RuleSet::RuleSet()
{
    // No resources to initialize in this implementation
}

RuleSet::~RuleSet()
{
    // No resources to clean up in this implementation
}

int RuleSet::countNeighbors(const Grid& grid, int x, int y) const
{
    int count = 0;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0) continue; // Skip the current cell

            int nx = x + dx;
            int ny = y + dy;

            // Check if the neighbor's coordinates are within the grid boundaries
            if (nx >= 0 && nx < grid.getWidth() && ny >= 0 && ny < grid.getHeight())
            {
                if (grid.getCell(nx, ny).isAlive())
                {
                    count++;
                }
            }
        }
    }

    return count;
}

bool RuleSet::applyRuleChosen(const Grid& grid, int x, int y) const
{
    int neighbors = countNeighbors(grid, x, y);
    bool currentState = grid.getCell(x, y).isAlive();

    // Implement a specific rule based on the cell's state and its neighbors
    // Example: Conway's Game of Life rules
    if (currentState)
    {
        // Cell is alive
        if (neighbors < 2 || neighbors > 3)
        {
            return false; // Cell dies due to underpopulation or overpopulation
        }
        else
        {
            return true; // Cell survives
        }
    }
    else
    {
        // Cell is dead
        if (neighbors == 3)
        {
            return true; // Cell becomes alive due to reproduction
        }
        else
        {
            return false; // Cell remains dead
        }
    }
}