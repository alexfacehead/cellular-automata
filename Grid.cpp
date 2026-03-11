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
    m_cells.assign(m_width, std::vector<Cell>(m_height));
}

void Grid::update(const RuleSet& ruleSet) {
    std::vector<std::vector<Cell>> updatedCells = m_cells;

    for (int x = 0; x < m_width; ++x) {
        for (int y = 0; y < m_height; ++y) {
            bool wasAlive = m_cells[x][y].isAlive();
            bool nowAlive = ruleSet.applyRuleChosen(*this, x, y);
            updatedCells[x][y].setAlive(nowAlive);
            if (nowAlive)
                updatedCells[x][y].setAge(wasAlive ? m_cells[x][y].getAge() + 1 : 1);
            else
                updatedCells[x][y].setAge(0);
        }
    }

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

void Grid::initializeCellsWithDensity(int densityPercent)
{
    m_cells.resize(m_width, std::vector<Cell>(m_height));
    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            bool state = (rand() % 100) < densityPercent;
            m_cells[x][y].setAlive(state);
        }
    }
}

void Grid::initializeCellsSeed()
{
    m_cells.assign(m_width, std::vector<Cell>(m_height));
    m_cells[m_width / 2][m_height / 2].setAlive(true);
}

void Grid::initializeCellsCluster()
{
    m_cells.assign(m_width, std::vector<Cell>(m_height));
    int cx = m_width / 2;
    int cy = m_height / 2;
    for (int x = cx - 3; x <= cx + 3; ++x)
        for (int y = cy - 3; y <= cy + 3; ++y)
            if (x >= 0 && x < m_width && y >= 0 && y < m_height)
                m_cells[x][y].setAlive(rand() % 2);
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