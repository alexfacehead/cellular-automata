#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

// Graphical library includes
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

// Custom includes
#include "Cell.h"
#include "Grid.h"
#include "RuleSet.h"
#include "Menu.h"
#include "Settings.h"

// Function prototypes
void initializeGrid(Grid& grid, int width, int height, int cellSize);
void applyRules(Grid& grid, const RuleSet& rules);
void render(sf::RenderWindow& window, const Grid& grid);
void handleInput(sf::RenderWindow& window, Grid& grid, RuleSet& rules, int cellSize, Menu& menu);

// static global vars
// Define the global variables in Main.cpp
int simulationSpeed = 0.15f;
bool paused = false;
bool settingsShown = false;
Settings settings;

sf::Font font;
// Create text objects to represent the menu options
sf::Text restartText("Restart Simulation & Apply Rules", font);
sf::Text resumeText("Resume Simulation", font);
sf::Text settingsText("Settings", font);
// Create a clock to control the simulation speed
sf::Clock simulationClock;

int main()
{
    int gridWidth = windowWidth / cellSize; // Make this variable non-constant so it can be changed in settings
    int gridHeight = windowHeight / cellSize; // Make this variable non-constant so it can be changed in settings
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

    // Create the grid and rules
    Grid grid(gridWidth, gridHeight); // Use the constructor that takes width and height parameters
    RuleSet rules;
    // Create a Menu instance
    Menu menu(grid, rules);
    // Initialize the grid with a custom pattern
    grid.initializeCellsPattern(customPattern);
    menu.toggle();

    // Main loop
    while (window.isOpen())
    {
        if (!menu.isShown() && !settingsShown) {
            // Handle input
            handleInput(window, grid, rules, cellSize, menu);

            // Apply rules only if the simulation is not paused and enough time has passed since the last update
            if (!paused && simulationClock.getElapsedTime().asSeconds() >= simulationSpeed) { // Use ::simulationClock to access the global simulationClock object
                applyRules(grid, rules);
                ::simulationClock.restart(); // Reset the simulationClock
            }
        }
        else if (menu.isShown()) {
            // Handle menu input
            menu.handleMenuInput(window);

            // Show menu only if the settings are not shown
            if (!settingsShown) {
                menu.showMenu(window);
            }
        }
        else if (settingsShown) {
            // Handle settings input
            menu.handleSettingsInput(window);

            // Show settings only if the menu is shown
            if (menu.isShown()) {
                menu.showSettings(window);
            }
        }

        // Render only if the menu and settings are not shown
        if (!menu.isShown() && !settingsShown) {
            window.clear();
            render(window, grid);
            window.display();
        }
        
    }

    return 0;
}

// This function is not needed as the grid constructor and initializer methods do the same thing
/*
void initializeGrid(Grid& grid, int width, int height, int cellSize)
{
}
*/

void applyRules(Grid& grid, const RuleSet& rules)
{
    // Use the update method of the grid class to apply the rules chosen by the user in settings
    grid.update(rules);
}

void render(sf::RenderWindow& window, const Grid& grid)
{
    // Create a rectangle shape to represent a cell
    sf::RectangleShape cellShape(sf::Vector2f(cellSize - 1, cellSize - 1));

    // Loop through the cells of the grid and draw them according to their state
    for (int x = 0; x < grid.getWidth(); ++x)
    {
        for (int y = 0; y < grid.getHeight(); ++y)
        {
            // Set the position of the cell shape according to the cell coordinates
            cellShape.setPosition(x * cellSize + 1, y * cellSize + 1);

            // Set the color of the cell shape according to the cell state (alive or dead)
            if (grid.getCell(x, y).isAlive())
            {
                cellShape.setFillColor(sf::Color::White);
            }
            else
            {
                cellShape.setFillColor(sf::Color::Black);
            }

            // Draw the cell shape on the window
            window.draw(cellShape);
        }
    }
}

void handleInput(sf::RenderWindow& window, Grid& grid, RuleSet& rules, int cellSize, Menu& menu)
{
    // Declare an event variable to store events from the window
    sf::Event event;

    // Poll events from the window until there are none left
    while (window.pollEvent(event))
    {
        // Handle different types of events
        switch (event.type)
        {
            case sf::Event::Closed: // The user closed the window
                window.close(); // Close the window
                break;

            case sf::Event::KeyPressed: // The user pressed a key on the keyboard
                if (event.key.code == sf::Keyboard::Escape) // The user pressed escape key
                {
                    window.close(); // Close the window
                }
                else if (event.key.code == sf::Keyboard::R) // The user pressed R key
                {
                    grid.initializeCells(); // Reinitialize the grid with random states
                }
                else if (event.key.code == sf::Keyboard::P) // The user pressed P key
                {
                    paused = !paused; // Toggle the paused flag
                }
                else if (event.key.code == sf::Keyboard::M) // The user pressed M key
                {
                    menu.toggle(); // Toggle the menu visibility using the Menu class method
                    paused = menu.isShown(); // Pause the simulation if the menu is shown
                }
                break;

            case sf::Event::MouseButtonPressed: // The user pressed a mouse button
                if (event.mouseButton.button == sf::Mouse::Left) // The user pressed left mouse button
                {
                    // Get the mouse position relative to the window
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    // Calculate the cell coordinates corresponding to the mouse position
                    int cellX = mouseX / cellSize;
                    int cellY = mouseY / cellSize;

                    // Check if the cell coordinates are within the grid boundaries
                    if (cellX >= 0 && cellX < grid.getWidth() && cellY >= 0 && cellY < grid.getHeight())
                    {
                        // Toggle the state of the cell at the mouse position
                        bool currentState = grid.getCell(cellX, cellY).isAlive();
                        grid.setCell(cellX, cellY, !currentState);
                    }
                }
                break;

            default: // Other types of events are ignored
                break;
        }
    }
}