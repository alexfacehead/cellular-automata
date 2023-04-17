#include "Cell.h"

Cell::Cell(int x, int y, bool initialState)
    : m_alive(initialState), m_neighbors(0), m_x(x), m_y(y)
{
}

Cell::~Cell()
{
}

bool Cell::isAlive() const
{
    return m_alive;
}

bool Cell::setAlive(bool alive)
{
    bool stateChanged = (m_alive != alive);
    m_alive = alive;
    return stateChanged;
}

bool Cell::setDead()
{
    bool wasAlive = isAlive();
    m_alive = !dead;  
    return wasAlive;
}


int Cell::getNeighbors() const
{
    return m_neighbors;
}


int Cell::getX() const
{
    return m_x;
}

int Cell::getY() const
{
    return m_y;
}

void Cell::setNeighbors(int neighbors)
{
    m_neighbors = neighbors;
}