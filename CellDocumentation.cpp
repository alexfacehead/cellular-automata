#include "Cell.h"

/**
 * Constructs a Cell object with the given coordinates and initial state.
 * 
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @param initialState The initial state of the cell (true for alive, false for dead).
 */
Cell::Cell(int x, int y, bool initialState)
    : m_alive(initialState), m_neighbors(0), m_x(x), m_y(y)
{
}

/**
 * Destructor for the Cell object.
 */
Cell::~Cell()
{
}

/**
 * Returns the current state of the cell.
 * 
 * @return true if the cell is alive, false if the cell is dead.
 */
bool Cell::isAlive() const
{
    return m_alive;
}

/**
 * Sets the state of the cell.
 * 
 * @param alive The new state of the cell (true for alive, false for dead).
 * @return true if the state changed, false if the state remained the same.
 */
bool Cell::setAlive(bool alive)
{
    bool stateChanged = (m_alive != alive);
    m_alive = alive;
    return stateChanged;
}

/**
 * Sets the cell to dead.
 * 
 * @return true if the cell was previously alive, false if it was already dead.
 */
bool Cell::setDead()
{
    bool wasAlive = isAlive();
    m_alive = !dead;  
    return wasAlive;
}

/**
 * Returns the number of neighbors for the cell.
 * 
 * @return The number of neighbors for the cell.
 */
int Cell::getNeighbors() const
{
    return m_neighbors;
}

/**
 * Returns the x-coordinate of the cell.
 * 
 * @return The x-coordinate of the cell.
 */
int Cell::getX() const
{
    return m_x;
}

/**
 * Returns the y-coordinate of the cell.
 * 
 * @return The y-coordinate of the cell.
 */
int Cell::getY() const
{
    return m_y;
}

/**
 * Sets the number of neighbors for the cell.
 * 
 * @param neighbors The new number of neighbors for the cell.
 */
void Cell::setNeighbors(int neighbors)
{
    m_neighbors = neighbors;
}