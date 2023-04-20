// Include the header files
#include "Menu.h"
#include "Grid.h"
#include "RuleSet.h"
#include <SFML/Graphics.hpp>
#include <filesystem>

// Use the sf namespace
using namespace sf;

// Define a constructor that takes a reference to a Grid object and a reference to a RuleSet object as parameters
// Initialize grid and rules using a member initializer list
Menu::Menu(Grid& grid, RuleSet& rules) : grid(grid), rules(rules)
{
    
    std::filesystem::path resourcesPath = "/home/dev/cellular-automata-2D/resources";
    font.loadFromFile((resourcesPath / "arial.ttf").string());

    // Set the font for the text objects
    restartText.setFont(font);
    resumeText.setFont(font);
    settingsText.setFont(font);

    // Set the string for the text objects
    restartText.setString("Restart Simulation & Apply Rules");
    resumeText.setString("Resume Simulation");
    settingsText.setString("Settings");

    // Set the font for the text objects
    windowSizeText.setFont(font);
    cellSizeText.setFont(font);
    simulationSpeedText.setFont(font);
    rulesText.setFont(font);

    // Set the string for the text objects
    windowSizeText.setString("Window Size:");
    cellSizeText.setString("Cell Size:");
    simulationSpeedText.setString("Simulation Speed:");
    rulesText.setString("Rules:");

    // Set the menu visibility to false by default
    shown = false;
}

// bool Menu::isMenuShown() {
//     return menuShown;
// }

void Menu::showMenu(sf::RenderWindow& window) 
{
    // Create a rectangle shape to represent the menu background
    sf::RectangleShape menuBackground(sf::Vector2f(windowWidth / 2, windowHeight / 2));
    
    // Set the position and color of the menu background shape
    menuBackground.setPosition(windowWidth / 4, windowHeight / 4);
    menuBackground.setFillColor(sf::Color(128, 128, 128));

    // Draw the menu background shape on the window
    window.draw(menuBackground);

    // Set the character size and color of the text
    restartText.setCharacterSize(24);
    resumeText.setCharacterSize(24);
    settingsText.setCharacterSize(24);

    restartText.setFillColor(sf::Color::White);
    resumeText.setFillColor(sf::Color::White);
    settingsText.setFillColor(sf::Color::White);

    // Set the origin of the text to the center
    restartText.setOrigin(restartText.getLocalBounds().width / 2, restartText.getLocalBounds().height / 2);
    resumeText.setOrigin(resumeText.getLocalBounds().width / 2, resumeText.getLocalBounds().height / 2);
    settingsText.setOrigin(settingsText.getLocalBounds().width / 2, settingsText.getLocalBounds().height / 2);

    // Set the position of the text relative to the menu background
    restartText.setPosition(menuBackground.getPosition().x + menuBackground.getSize().x / 2, menuBackground.getPosition().y + menuBackground.getSize().y / 4);
    resumeText.setPosition(menuBackground.getPosition().x + menuBackground.getSize().x / 2, menuBackground.getPosition().y + menuBackground.getSize().y / 2);
    settingsText.setPosition(menuBackground.getPosition().x + menuBackground.getSize().x / 2, menuBackground.getPosition().y + menuBackground.getSize().y * 3 / 4);

    // Draw the text on the window
    window.draw(restartText);
    window.draw(resumeText);
    window.draw(settingsText);

    // Display the window
    window.display();
}

void Menu::toggle() {
    shown = !shown;
}

bool Menu::isShown() {
    return shown;
}

void Menu::handleMenuInput(sf::RenderWindow& window) // Remove unused parameters
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
                else if (event.key.code == sf::Keyboard::M) // The user pressed M key
                {
                    this->toggle(); // Toggle the menu using this pointer
                    paused = this->isShown(); // Pause the simulation if the menu is shown using this pointer
                }
                break;

            case sf::Event::MouseButtonPressed: // The user pressed a mouse button
                int mouseX = event.mouseButton.x; // Declare and initialize mouseX variable
                int mouseY = event.mouseButton.y; // Declare and initialize mouseY variable
                if (event.mouseButton.button == sf::Mouse::Left) // The user pressed left mouse button
                {
                    // Get the mouse position relative to the window

                    // Check if the mouse position is within the restart text bounds
                    if (mouseX >= restartText.getGlobalBounds().left && mouseX <= restartText.getGlobalBounds().left + restartText.getGlobalBounds().width && mouseY >= restartText.getGlobalBounds().top && mouseY <= restartText.getGlobalBounds().top + restartText.getGlobalBounds().height)
                    {
                        // Restart the simulation and apply the rules chosen by the user in settings
                        grid.initializeCells();
                        grid.update(rules);
                        ::simulationClock.restart(); // Reset the simulationClock

                        paused = false; // Resume the simulation
                        this->toggle(); // Hide the menu using this pointer
                    }

                    // Check if the mouse position is within the resume text bounds
                    else if (mouseX >= resumeText.getGlobalBounds().left && mouseX <= resumeText.getGlobalBounds().left + resumeText.getGlobalBounds().width &&
                        mouseY >= resumeText.getGlobalBounds().top && mouseY <= resumeText.getGlobalBounds().top + resumeText.getGlobalBounds().height)
                    {
                        // Resume the simulation and hide the menu
                        paused = false;
                        this->toggle(); // Hide the menu using this pointer
                    }

                    // Check if the mouse position is within the settings text bounds
                    else if (mouseX >= settingsText.getGlobalBounds().left && mouseX <= settingsText.getGlobalBounds().left + settingsText.getGlobalBounds().width &&
                        mouseY >= settingsText.getGlobalBounds().top && mouseY <= settingsText.getGlobalBounds().top + settingsText.getGlobalBounds().height)
                    {
                        // Show the settings and hide the menu
                        settingsShown = true;
                        this->toggle(); // Hide the menu using this pointer
                    }
                }
                break;
        } // Add a closing brace for the switch statement
    } // Add a closing brace for the while loop
}

void Menu::showSettings(sf::RenderWindow& window) 
{
    // Create a rectangle shape to represent the settings background
    sf::RectangleShape settingsBackground(sf::Vector2f(windowWidth / 2, windowHeight / 2));
    
    // Set the position and color of the settings background shape
    settingsBackground.setPosition(windowWidth / 4, windowHeight / 4);
    settingsBackground.setFillColor(sf::Color(128, 128, 128));

    // Draw the settings background shape on the window
    window.draw(settingsBackground);

    // Set the character size and color of the text
    windowSizeText.setCharacterSize(24);
    cellSizeText.setCharacterSize(24);
    simulationSpeedText.setCharacterSize(24);
    rulesText.setCharacterSize(24);

    windowSizeText.setFillColor(sf::Color::White);
    cellSizeText.setFillColor(sf::Color::White);
    simulationSpeedText.setFillColor(sf::Color::White);
    rulesText.setFillColor(sf::Color::White);

    // Set the origin of the text to the center
    windowSizeText.setOrigin(windowSizeText.getLocalBounds().width / 2, windowSizeText.getLocalBounds().height / 2);
    cellSizeText.setOrigin(cellSizeText.getLocalBounds().width / 2, cellSizeText.getLocalBounds().height / 2);
    simulationSpeedText.setOrigin(simulationSpeedText.getLocalBounds().width / 2, simulationSpeedText.getLocalBounds().height / 2);
    rulesText.setOrigin(rulesText.getLocalBounds().width / 2, rulesText.getLocalBounds().height / 2);

    // Set the position of the text relative to the settings background
    windowSizeText.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y / 8);
    cellSizeText.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y * 3 / 8);
    simulationSpeedText.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y * 5 / 8);
    rulesText.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y * 7 / 8);

    // Draw the text on the window
    window.draw(windowSizeText);
    window.draw(cellSizeText);
    window.draw(rulesText);

    // Create text objects to represent the settings values
    sf::Text windowSizeValue(std::to_string(windowWidth) + " x " + std::to_string(windowHeight), font);
    sf::Text cellSizeValue(std::to_string(cellSize), font);
    sf::Text simulationSpeedValue(std::to_string(simulationSpeed), font);
    sf::Text rulesValue("Conway", font); // Use a placeholder value for now

    // Set the character size and color of the text
    windowSizeValue.setCharacterSize(24);
    cellSizeValue.setCharacterSize(24);
    simulationSpeedValue.setCharacterSize(24);
    rulesValue.setCharacterSize(24);

    windowSizeValue.setFillColor(sf::Color::White);
    cellSizeValue.setFillColor(sf::Color::White);
    simulationSpeedValue.setFillColor(sf::Color::White);
    rulesValue.setFillColor(sf::Color::White);

    // Set the origin of the text to the center
    windowSizeValue.setOrigin(windowSizeValue.getLocalBounds().width / 2, windowSizeValue.getLocalBounds().height / 2);
    cellSizeValue.setOrigin(cellSizeValue.getLocalBounds().width / 2, cellSizeValue.getLocalBounds().height / 2);
    simulationSpeedValue.setOrigin(simulationSpeedValue.getLocalBounds().width / 2, simulationSpeedValue.getLocalBounds().height / 2);
    rulesValue.setOrigin(rulesValue.getLocalBounds().width / 2, rulesValue.getLocalBounds().height / 2);

    // Set the position of the text relative to the settings background
    windowSizeValue.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x * 3 / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y / 8);
    cellSizeValue.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x * 3 / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y * 3 / 8);
    simulationSpeedValue.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x * 3 / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y * 5 / 8);
    rulesValue.setPosition(settingsBackground.getPosition().x + settingsBackground.getSize().x * 3 / 4, settingsBackground.getPosition().y + settingsBackground.getSize().y * 7 / 8);

    // Draw the text on the window
    window.draw(windowSizeValue);
    window.draw(cellSizeValue);
    window.draw(simulationSpeedValue);
    window.draw(rulesValue);

    // Display the window
    window.display();
}

void Menu::handleSettingsInput(sf::RenderWindow& window) // Remove unused parameters
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
                else if (event.key.code == sf::Keyboard::M) // The user pressed M key
                {
                    this->toggle(); // Toggle the menu using this pointer
                    paused = this->isShown(); // Pause the simulation if the menu is shown using this pointer
                    settingsShown = false; // Hide the settings using this pointer
                }
                break;

            case sf::Event::MouseButtonPressed: // The user pressed a mouse button
                if (event.mouseButton.button == sf::Mouse::Left) // The user pressed left mouse button
                {
                    // Get the mouse position relative to the window
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    // TODO: Implement a way to change the settings values by clicking on them and entering new values

                    /*
                    Example pseudocode:

                    if (mouseX and mouseY are within the bounds of windowSizeValue) {
                        prompt for new width and height values;
                        validate and update windowWidth and windowHeight variables;
                        resize and reinitialize grid;
                        resize and clear window;
                    }

                    if (mouseX and mouseY are within the bounds of cellSizeValue) {
                        prompt for new cell size value;
                        validate and update cellSize variable;
                        recalculate and update gridWidth and gridHeight variables;
                        resize and reinitialize grid 
                        prompt for new cell size value;
                        validate and update cellSize variable;
                        recalculate and update gridWidth and gridHeight variables;
                        resize and reinitialize grid;
                        resize and clear window;
                    }

                    if (mouseX and mouseY are within the bounds of simulationSpeedValue) {
                        prompt for new simulation speed value;
                        validate and update simulationSpeed variable;
                    }

                    if (mouseX and mouseY are within the bounds of rulesValue) {
                        prompt for new rules choice from a list of options;
                        validate and update rules object with the chosen rule;
                    }
                    */
                }
                break;

            default: // Other types of events are ignored
                break;
        }
    }
}