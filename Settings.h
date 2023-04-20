// Include guard
#ifndef SETTINGS_H
#define SETTINGS_H

// Include the header files
#include <SFML/Graphics.hpp>
#include <vector>

class Settings
{
private:
    // Declare the settings vars
    int windowSizeWidth;
    int windowSizeHeight;
    int cellSize;
    float simulationSpeed;

    // Declare a private member variable for the custom pattern setting
    std::vector<std::vector<bool>> customPattern;

public:
    // Declare a constructor for the Settings class
    Settings();

    // Getters
    int getWindowSizeWidth();
    int getWindowSizeHeight();
    int getCellSize();
    float getSimulationSpeed();
    // Getter method for the custom pattern setting
    std::vector<std::vector<bool>> getCustomPattern();


    // Setters
    void setCellSize(int cellSize);
    void setWindowSizeWidth(int width);
    void setWindowSizeHeight(int height);
    void setSimulationSpeed(float speed);
    // Setter method for the custom pattern setting
    void setCustomPattern(std::vector<std::vector<bool>> pattern);

    // Declare a public method to read the custom pattern setting from a text file
    void readCustomPattern();

    // Declare a public method to write the custom pattern setting to a text file
    void writeCustomPattern();
};

#endif // SETTINGS_H