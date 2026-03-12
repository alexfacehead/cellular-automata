#ifndef MENU_H
#define MENU_H

#include "Grid.h"
#include "RuleSet.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

const int windowWidth = 800;
const int windowHeight = 600;
extern int cellSize;

extern float simulationSpeed;
extern sf::Clock simulationClock;
extern bool paused;

enum class MenuPage { RULES, CUSTOM };
enum class InitMode { RANDOM_50, RANDOM_25, RANDOM_10, RANDOM_5, SINGLE_SEED, CLUSTER };

struct SavedConfig {
    std::string name;
    bool birth[9];
    bool survival[9];
    int cellSz;
    float speed;
    InitMode init;
};

struct VisualItem {
    enum Type { HEADER, PRESET, CUSTOM_OPT, SAVED };
    Type type;
    int presetIndex;
    std::string label;
};

class Menu
{
private:
    sf::Font font;
    bool shown;
    Grid& grid;
    RuleSet& rules;

    MenuPage currentPage;
    int selectedIndex;
    int scrollOffset;
    std::vector<RuleSet> presets;
    std::vector<VisualItem> visualItems;

    InitMode initMode;
    bool lastWasCustom;

    bool customBirth[9];
    bool customSurvival[9];

    std::vector<SavedConfig> savedConfigs;

    void buildVisualItems();
    void showRulesPage(sf::RenderWindow& window);
    void handleRulesInput(sf::RenderWindow& window);
    void showCustomPage(sf::RenderWindow& window);
    void handleCustomInput(sf::RenderWindow& window);

    void initializeGrid();
    void selectPreset(int index);
    void selectSaved(int index);
    void applyCustomRule();
    void moveSelection(int dir);
    void loadSavedConfigs();
    void writeSavedConfigs();

public:
    static const char* initModeName(InitMode mode);
    static InitMode initModeFromInt(int v);
    Menu(Grid& grid, RuleSet& rules);
    bool isShown();
    void toggle();
    void saveCurrentConfig();
    void showMenu(sf::RenderWindow& window);
    void handleMenuInput(sf::RenderWindow& window);
};

#endif
