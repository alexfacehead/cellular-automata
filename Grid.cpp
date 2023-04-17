#include "Grid.h"

Grid::Grid()
    : m_width(0), m_height(0)
{
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
    initializeCells();
}

void Grid::update()
{
    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            m_cells[x][y].update();
        }
    }
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
    for (size_t x = 0; x < pattern.size(); ++x)
    {
        for (size_t y = 0; y < pattern[x].size(); ++y)
        {
            m_cells[startX + x][startY + y].setAlive(pattern[x][y]);
        }
    }
}