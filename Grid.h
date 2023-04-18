#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Cell.h"
#include "RuleSet.h"

class Grid
{
public:
    Grid(int width, int height);
    Grid();
    ~Grid();

    int getWidth() const;
    int getHeight() const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const; // Add this line
    void setCell(int x, int y, bool state);
    void resize(int width, int height);
    void update(const RuleSet& ruleSet); // Modify the function signature to accept a RuleSet reference

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