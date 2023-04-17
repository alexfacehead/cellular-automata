#include "Grid.h"

/**
 * Default constructor for the Grid object.
 */
Grid::Grid()
    : m_width(0), m_height(0)
{
}

/**
 * Destructor for the Grid object.
 */
Grid::~Grid()
{
    // No resources to clean up in this implementation
}

/**
 * Returns the width of the grid.
 * 
 * @return The width of the grid.
 */
int Grid::getWidth() const
{
    return m_width;
}

/**
 * Returns the height of the grid.
 * 
 * @return The height of the grid.
 */
int Grid::getHeight() const
{
    return m_height;
}

/**
 * Returns a reference to the cell at the given coordinates.
 * 
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @return A reference to the cell at the given coordinates.
 */
Cell& Grid::getCell(int x, int y)
{
    return m_cells[x][y];
}

/**
 * Sets the state of the cell at the given coordinates.
 * 
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @param state The new state of the cell (true for alive, false for dead).
 */
void Grid::setCell(int x, int y, bool state)
{
    m_cells[x][y].setAlive(state);
}

/**
 * Resizes the grid to the given dimensions and initializes cells.
 * 
 * @param width The new width of the grid.
 * @param height The new height of the grid.
 */
void Grid::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    initializeCells();
}

/**
 * Updates the grid according to the current ruleset.
 */
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

/**
 * Initializes the grid cells with random initial states.
 */
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

/**
 * Initializes the grid cells with Conway's Game of Life initial states.
 */
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

/**
 * Initializes the grid cells with a custom initial pattern.
 */
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

/**
 * Initializes the grid cells with a specified pattern.
 * 
 * @param pattern A 2D vector of booleans representing the pattern to be placed at the center of the grid.
 */
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