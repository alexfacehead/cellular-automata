// Include guard
#ifndef SETTINGS_H
#define SETTINGS_H

// Include the header files
#include "Menu.h"
#include <SFML/Graphics.hpp>


class Settings
{
private:
    // Declare the settings vars
    int windowSizeWidth;
    int windowSizeHeight;
    int cellSize;
    float simulationSpeed;

public:
    // Getters
    int getWindowSizeWidth();
    int getWindowSizeHeight();
    int getCellSize();
    float getSimulationSpeed();


    // Setters
    void setCellSize(int cellSize);
    void setWindowSizeWidth(int width);
    void setWindowSizeHeight(int height);
    void setSimulationSpeed(float speed);
    
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