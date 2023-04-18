#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Cell.h"

class Grid
{
public:
    Grid(int width, int height);
    Grid();
    ~Grid();

    int getWidth() const;
    int getHeight() const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const; // Added const version of getCell
    void setCell(int x, int y, bool state);
    void resize(int width, int height);
    void update();

    void initializeCells();
    void initializeCellsConway();
    void initializeCellsCustom();
    void initializeCellsPattern(const std::vector<std::vector<bool>>& pattern);

private:
    int m_width;
    int m_height;
    std::vector<std::vector<Cell>> m_cells;
};

#endif