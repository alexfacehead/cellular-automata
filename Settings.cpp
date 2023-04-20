// Include the Settings header
#include "Settings.h"

// Include the fstream header
#include <fstream>
#include <iostream>

// Define the constructor for the Settings class
Settings::Settings()
{
    // Initialize the settings values with some default values
    windowSizeWidth = 800;
    windowSizeHeight = 600;
    cellSize = 10;
    simulationSpeed = 1.0f;

    // Read the custom pattern setting from a text file
    readCustomPattern();
}

// Define the getter method for window size width
int Settings::getWindowSizeWidth()
{
    // Return the window size width value
    return windowSizeWidth;
}

// Define the getter method for window size height
int Settings::getWindowSizeHeight()
{
    // Return the window size height value
    return windowSizeHeight;
}

// Define the getter method for cell size
int Settings::getCellSize()
{
    // Return the cell size value
    return cellSize;
}

// Define the getter method for simulation speed
float Settings::getSimulationSpeed()
{
    // Return the simulation speed value
    return simulationSpeed;
}

// Define the getter method for custom pattern
std::vector<std::vector<bool>> Settings::getCustomPattern()
{
    // Return the custom pattern value
    return customPattern;
}

// Define the setter method for cell size
void Settings::setCellSize(int cellSize)
{
    // Set the cell size value
    this->cellSize = cellSize;
}

// Define the setter method for window size width
void Settings::setWindowSizeWidth(int width)
{
    // Set the window size width value
    this->windowSizeWidth = width;
}

// Define the setter method for window size height
void Settings::setWindowSizeHeight(int height)
{
    // Set the window size height value
    this->windowSizeHeight = height;
}

// Define the setter method for simulation speed
void Settings::setSimulationSpeed(float speed)
{
    // Set the simulation speed value
    this->simulationSpeed = speed;
}

// Define the setter method for custom pattern
void Settings::setCustomPattern(std::vector<std::vector<bool>> pattern)
{
    // Set the custom pattern value
    this->customPattern = pattern;
}

// Define the public method to read the custom pattern setting from a text file
void Settings::readCustomPattern()
{
    // Declare an ifstream object to read from a file
    std::ifstream inputFile;

    // Open the file called settings.txt in your project's root directory
    inputFile.open("settings.txt");

    // Check if the file is open
    if (inputFile.is_open())
    {
        // Declare variables to store the data from the file
        int rows, cols;
        bool value;

        // Read the number of rows and columns from the first line of the file
        inputFile >> rows >> cols;

        // Resize the vector of vectors according to the number of rows and columns
        customPattern.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            customPattern[i].resize(cols);
        }

        // Read the values from the rest of the file and store them in the vector of vectors
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                inputFile >> value;
                customPattern[i][j] = value;
            }
        }

        // Close the file
        inputFile.close();
    }
    else
    {
        // The file could not be opened
        std::cout << "Error: Could not open settings.txt\n";
    }
}

// Define the public method to write the custom pattern setting to a text file
void Settings::writeCustomPattern()
{
    // Declare an ofstream object to write to a file
    std::ofstream outputFile;

    // Open or create a file called settings.txt in your project's root directory
    outputFile.open("settings.txt");

    // Check if the file is open
    if (outputFile.is_open())
    {
        // Get the number of rows and columns from the vector of vectors
        int rows = customPattern.size();
        int cols = customPattern[0].size();

        // Write the number of rows and columns to the first line of the file
        outputFile << rows << " " << cols << "\n";

        // Write the values from the vector of vectors to the rest of the file
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                outputFile << customPattern[i][j] << " ";
            }
            outputFile << "\n";
        }

        // Close the file
        outputFile.close();

    } // Add this closing curly brace to end the writeCustomPattern method
}