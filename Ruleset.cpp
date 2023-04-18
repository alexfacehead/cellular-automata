// RuleSet.cpp

#include "RuleSet.h"

RuleSet::RuleSet()
{
    // TODO: Implement constructor (initialize any necessary variables)
    // You can initialize your rule set here, or load it from a file
}

RuleSet::~RuleSet()
{
    // No resources to clean up in this example
}

void RuleSet::updateGrid(Grid& grid)
{
    Grid newGrid = grid; // Create a copy of the current grid to store the updated states

    for (int x = 0; x < grid.getWidth(); ++x)
    {
        for (int y = 0; y < grid.getHeight(); ++y)
        {
            bool newState = applyRule(grid, x, y);
            newGrid.setCell(x, y, newState);
            int neighbors = countNeighbors(grid, x, y);
            newGrid.getCell(x, y).setNeighbors(neighbors);
        }
    }

    grid = newGrid; // Update the original grid with the new states
}

int RuleSet::countNeighbors(const Grid& grid, int x, int y)
{
    // Get the grid size limitations
    const int height = grid.getHeight();
    const int width = grid.getWidth();

    // Initialize the cells surrounding the current cell
    const int left = x - 1;
    const int right = x + 1;
    const int up = y + 1;
    const int down = y - 1;
    int count = 0;

    // Check each neighboring cell and count the alive ones
    for (int i = left; i <= right; ++i)
    {
        for (int j = down; j <= up; ++j)
        {
            if (i >= 0 && i < width && j >= 0 && j < height)
            {
                if (!(i == x && j == y)) // Exclude the central cell
                {
                    count += grid.getCell(i, j).getState() ? 1 : 0;
                }
            }
        }
    }

    return count;
}

bool RuleSet::applyRule(const Grid& grid, int x, int y)
{
    // TODO: Implement applyRule (apply a specific rule based on the cell's state and its neighbors)
    // Example: Conway's Game of Life rules
    int aliveNeighbors = countNeighbors(grid, x, y);
    bool currentState = grid.getCell(x, y).getState();

    if (currentState)
    {
        // Rule 1: Any live cell with fewer than two live neighbors dies, as if by underpopulation.
        // Rule 2: Any live cell with two or three live neighbors lives on to the next generation.
        // Rule 3: Any live cell with more than three live neighbors dies, as if by overpopulation.
        return aliveNeighbors == 2 || aliveNeighbors == 3;
    }
    else
    {
        // Rule 4: Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
        return aliveNeighbors == 3;
    }
}