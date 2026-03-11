#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Cell.h"
#include "Grid.h"
#include "RuleSet.h"
#include "Menu.h"

void applyRules(Grid& grid, const RuleSet& rules);
void render(sf::RenderWindow& window, const Grid& grid, const RuleSet& rules);
void handleInput(sf::RenderWindow& window, Grid& grid, RuleSet& rules, Menu& menu);

float simulationSpeed = 0.15f;
bool paused = false;
int cellSize = 10;
sf::Clock simulationClock;

int main()
{
    std::srand(static_cast<unsigned>(std::time(0)));

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D Cellular Automata");
    window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

    Grid grid(windowWidth / cellSize, windowHeight / cellSize);
    RuleSet rules;
    Menu menu(grid, rules);
    menu.toggle();

    while (window.isOpen())
    {
        if (menu.isShown()) {
            menu.handleMenuInput(window);
            if (menu.isShown()) {
                menu.showMenu(window);
            }
        } else {
            handleInput(window, grid, rules, menu);

            if (!paused && simulationClock.getElapsedTime().asSeconds() >= simulationSpeed) {
                applyRules(grid, rules);
                simulationClock.restart();
            }

            window.clear();
            render(window, grid, rules);
            window.display();
        }
    }

    return 0;
}

static const float speedSteps[] = { 0.02f, 0.05f, 0.08f, 0.10f, 0.15f, 0.25f, 0.40f };
static const int numSpeedSteps = 7;

static int findSpeed(float s) {
    int best = 0; float d = 999.f;
    for (int i = 0; i < numSpeedSteps; i++) {
        float dd = std::abs(speedSteps[i] - s);
        if (dd < d) { d = dd; best = i; }
    }
    return best;
}

void applyRules(Grid& grid, const RuleSet& rules)
{
    grid.update(rules);
}

sf::Font& getFont()
{
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        font.loadFromFile("resources/arial.ttf");
        loaded = true;
    }
    return font;
}

static sf::Color ageToColor(int age)
{
    if (age <= 0) return sf::Color::Black;
    if (age > 80) age = 80;

    // white → cyan → blue → purple → deep magenta
    if (age <= 3) {
        // white → cyan
        float t = (age - 1) / 2.f;
        return sf::Color(255 - (int)(255 * t), 255, 255);
    }
    if (age <= 15) {
        // cyan → blue
        float t = (age - 3) / 12.f;
        return sf::Color(0, 255 - (int)(180 * t), 255);
    }
    if (age <= 40) {
        // blue → purple
        float t = (age - 15) / 25.f;
        return sf::Color((int)(160 * t), 75 - (int)(45 * t), 255);
    }
    // purple → deep magenta
    float t = (age - 40) / 40.f;
    return sf::Color(160 + (int)(40 * t), 30 - (int)(15 * t), 255 - (int)(35 * t));
}

void render(sf::RenderWindow& window, const Grid& grid, const RuleSet& rules)
{
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;
    static int lastW = 0, lastH = 0;

    int w = grid.getWidth();
    int h = grid.getHeight();

    if (w != lastW || h != lastH) {
        image.create(w, h, sf::Color::Black);
        texture.create(w, h);
        sprite.setTexture(texture, true);
        lastW = w;
        lastH = h;
    }

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            image.setPixel(x, y, ageToColor(grid.getCell(x, y).getAge()));

    texture.update(image);
    sprite.setScale((float)windowWidth / w, (float)windowHeight / h);
    window.draw(sprite);

    // End button overlay
    sf::RectangleShape endBtn(sf::Vector2f(60, 26));
    endBtn.setPosition(windowWidth - 68, 8);
    endBtn.setFillColor(sf::Color(80, 50, 30));
    endBtn.setOutlineColor(sf::Color(180, 120, 60));
    endBtn.setOutlineThickness(1);
    window.draw(endBtn);

    sf::Text endText("End", getFont(), 14);
    endText.setFillColor(sf::Color(255, 220, 180));
    endText.setPosition(windowWidth - 52, 12);
    window.draw(endText);

    // HUD - top left info bar
    char speedBuf[16];
    std::snprintf(speedBuf, sizeof(speedBuf), "%.2fs", simulationSpeed);
    std::string hudStr = rules.name + "  " + rules.getNotation()
        + "   |   " + speedBuf + "/step";
    if (paused) hudStr += "   [PAUSED]";

    sf::Text hudText(hudStr, getFont(), 11);
    float hudW = hudText.getGlobalBounds().width + 16;
    sf::RectangleShape hudBg(sf::Vector2f(hudW, 20));
    hudBg.setPosition(0, 0);
    hudBg.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(hudBg);

    hudText.setFillColor(sf::Color(170, 180, 200));
    hudText.setPosition(8, 3);
    window.draw(hudText);
}

void handleInput(sf::RenderWindow& window, Grid& grid, RuleSet& rules, Menu& menu)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::Resized:
                window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::R)
                    grid.initializeCells();
                else if (event.key.code == sf::Keyboard::P)
                    paused = !paused;
                else if (event.key.code == sf::Keyboard::M) {
                    menu.toggle();
                    paused = menu.isShown();
                }
                else if (event.key.code == sf::Keyboard::Equal) {
                    int i = findSpeed(simulationSpeed);
                    if (i > 0) simulationSpeed = speedSteps[i - 1];
                }
                else if (event.key.code == sf::Keyboard::Hyphen) {
                    int i = findSpeed(simulationSpeed);
                    if (i < numSpeedSteps - 1) simulationSpeed = speedSteps[i + 1];
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f pos = window.mapPixelToCoords(
                        sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    int mx = (int)pos.x;
                    int my = (int)pos.y;

                    // End button check
                    if (mx >= windowWidth - 68 && mx <= windowWidth - 8 && my >= 8 && my <= 34) {
                        menu.toggle();
                        paused = true;
                        break;
                    }

                    int cellX = mx / cellSize;
                    int cellY = my / cellSize;
                    if (cellX >= 0 && cellX < grid.getWidth() && cellY >= 0 && cellY < grid.getHeight())
                        grid.setCell(cellX, cellY, !grid.getCell(cellX, cellY).isAlive());
                }
                break;

            default:
                break;
        }
    }
}
