// Include guard
#ifndef MENU_H
#define MENU_H

// Include the header files
#include "Grid.h"
#include "RuleSet.h"
#include <SFML/Graphics.hpp>

// Use the sf namespace
using namespace sf;

// Define some constants for window size and cell size
const int windowWidth = 800;
const int windowHeight = 600;
const int cellSize = 10;

// Define a global variable for simulation speed
extern int simulationSpeed;

// Define a global variable for simulation clock
extern sf::Clock simulationClock;

// Define a global variable for paused state
extern bool paused;

// Define a global variable for settings shown state
extern bool settingsShown;

class Menu
{
private:
    // Declare some text objects to represent the menu options
    sf::Text restartText;
    sf::Text resumeText;
    sf::Text settingsText;

    // Declare some text objects to represent the settings labels
    sf::Text windowSizeText;
    sf::Text cellSizeText;
    sf::Text simulationSpeedText;
    sf::Text rulesText;

    // Declare a font object to load and store the font
    sf::Font font;

    // Declare a boolean variable to store the menu visibility
    bool shown;

    // Declare a reference to a Grid object
    Grid& grid;

    // Declare a reference to a RuleSet object
    RuleSet& rules;

public:
    bool isMenuShown();
    
    // Declare a constructor that takes a reference to a Grid object and a reference to a RuleSet object as parameters
    Menu(Grid& grid, RuleSet& rules);

    // Declare a method that returns the menu visibility
    bool isShown();

    // Declare a method that toggles the menu visibility
    void toggle();

    // Declare a method that displays the menu on the window
    void showMenu(sf::RenderWindow& window);

    // Declare a method that handles the user input for the menu
    void handleMenuInput(sf::RenderWindow& window);

    // Declare a method that displays the settings on the window
    void showSettings(sf::RenderWindow& window);

    // Declare a method that handles the user input for the settings
    void handleSettingsInput(sf::RenderWindow& window);
};

#endif // MENU_H