#pragma once

class Cell
{
public:
    Cell();
    Cell(int x, int y, bool initialState);
    ~Cell();

    // Getters and setters
    bool isAlive() const;
    bool setAlive(bool alive);
    bool setDead();
    int getNeighbors() const;
    void setNeighbors(int neighbors);
    int getX() const;
    int getY() const;

private:
    bool m_alive;
    int m_neighbors;
    int m_x;
    int m_y;
};