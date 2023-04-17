#pragma once

#include <vector>
#include "Cell.h"

class Grid
{
public:
    Grid();
    ~Grid();

    // Getters and setters
    int getWidth() const;
    int getHeight() const;
    Cell& getCell(int x, int y);
    void setCell(int x, int y, bool state);

    // Grid manipulation
    void resize(int width, int height);
    void update();

    // Special grid initializations
    void initializeCellsConway();
    void initializeCellsCustom();
    void initializeCellsPattern(const std::vector<std::vector<bool>>& pattern);

private:
    int m_width;
    int m_height;
    std::vector<std::vector<Cell>> m_cells;

    // Helper functions
    void initializeCells();
};