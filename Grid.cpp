#include "Cell.h" // Added missing include directive
#include <cstdlib> // Include this header for rand() function
#include <cstddef> // Include this header for std::
#include "Grid.h"

Grid::Grid(int width, int height)
    : m_width(width), m_height(height) // Set the width and height according to the parameters
{
    initializeCells(); // Initialize the cells when creating the grid
}

Grid::Grid()
    : m_width(1000), m_height(1000) // Set the width and height to 1000 by default
{
    initializeCells(); // Initialize the cells when creating the grid
}

Grid::~Grid()
{
    // No resources to clean up in this implementation
}

int Grid::getWidth() const
{
    return m_width;
}

int Grid::getHeight() const
{
    return m_height;
}

const Cell& Grid::getCell(int x, int y) const {
    return m_cells[x][y];
}

Cell& Grid::getCell(int x, int y)
{
    return m_cells[x][y];
}

void Grid::setCell(int x, int y, bool state)
{
    m_cells[x][y].setAlive(state);
}

void Grid::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void Grid::update(const RuleSet& ruleSet) {
    // Create a temporary grid to store the updated cell states
    std::vector<std::vector<Cell>> updatedCells = m_cells;

    for (int x = 0; x < m_width; ++x) {
        for (int y = 0; y < m_height; ++y) {
            // Apply the rules to the cell at (x, y) and store the result in the temporary grid
            updatedCells[x][y].setAlive(ruleSet.applyRuleChosen(*this, x, y));
        }
    }

    // Replace the current grid with the updated grid
    m_cells = updatedCells;
}

// Random ruleset
void Grid::initializeCells()
{
    m_cells.resize(m_width, std::vector<Cell>(m_height));
    // Initialize cell states here, e.g., with random initial states
    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            // Set the cell state to a random value (true or false)
            bool state = rand() % 2;
            m_cells[x][y].setAlive(state);
        }
    }
}

// Conways game of life initial rules
void Grid::initializeCellsConway()
{
    m_cells.resize(m_width, std::vector<Cell>(m_height));
    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            // Set the cell state to a random value (true or false)
            bool state = rand() % 2;
            m_cells[x][y].setAlive(state);
        }
    }
}

// Random rule set for testing
void Grid::initializeCellsCustom()
{
    m_cells.resize(m_width, std::vector<Cell>(m_height));
    // Customize the initial state of cells as desired
    // For example, create a pattern of cells in the center of the grid
    int centerX = m_width / 2;
    int centerY = m_height / 2;
    
    m_cells[centerX][centerY].setAlive(true);
    m_cells[centerX + 1][centerY].setAlive(true);
    m_cells[centerX][centerY + 1].setAlive(true);
    m_cells[centerX - 1][centerY + 1].setAlive(true);
    m_cells[centerX + 1][centerY + 1].setAlive(true);
}


// Set a particular pattern by passing an input 3X3 vector of Bools
void Grid::initializeCellsPattern(const std::vector<std::vector<bool>>& pattern)
{
    m_cells.resize(m_width, std::vector<Cell>(m_height));
    
    // Calculate the starting position to place the pattern at the center of the grid
    int startX = (m_width - pattern.size()) / 2;
    int startY = (m_height - pattern[0].size()) / 2;

    // Set the cell states according to the given pattern
    for (std::size_t x = 0; x < pattern.size(); ++x)
    {
        for (std::size_t y = 0; y < pattern[x].size(); ++y)
        {
            m_cells[startX + x][startY + y].setAlive(pattern[x][y]);
        }
    }
}