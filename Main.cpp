// Main.cpp

// Required includes
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

// Graphical library includes
#include <SFML/Graphics.hpp>

// Custom includes
#include "Cell.h"
#include "Grid.h"
#include "RuleSet.h"

// Function prototypes
void initializeGrid(Grid& grid, int width, int height, int cellSize);
void applyRules(Grid& grid, const RuleSet& rules);
void render(sf::RenderWindow& window, const Grid& grid);
void handleInput(sf::RenderWindow& window, Grid& grid, RuleSet& rules);

int main()
{

    
    // Constants
    const int windowWidth = 800;
    const int windowHeight = 600;
    const int cellSize = 10;
    const int gridWidth = windowWidth / cellSize;
    const int gridHeight = windowHeight / cellSize;
    const std::string windowTitle = "2D Cellular Automata";

    // Custom init rules
    const std::vector<std::vector<bool>> customPattern = {
    {false, true, false},
    {true, true, true},
    {false, true, false}
    };

    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(0)));

    // Create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle);

    // Create the grid, cells, and rules
    Grid grid;
    RuleSet rules;
    initializeGrid(grid, gridWidth, gridHeight, cellSize);

    // Main loop
    while (window.isOpen())
    {
        // Handle input
        handleInput(window, grid, rules);

        // Apply rules
        applyRules(grid, rules);

        // Render
        window.clear();
        render(window, grid);
        window.display();
    }

    return 0;
}

// Graphical setup function implementations

void initializeGrid(Grid& grid, int width, int height, int cellSize)
{
    // TODO: Implement grid initialization
}

void applyRules(Grid& grid, const RuleSet& rules)
{
    // TODO: Implement rule application
}

void render(sf::RenderWindow& window, const Grid& grid)
{
    // TODO: Implement rendering
}

void handleInput(sf::RenderWindow& window, Grid& grid, RuleSet& rules)
{
    // TODO: Implement input handling
}