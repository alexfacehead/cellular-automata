#include "Menu.h"
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>

using namespace sf;

// --- Speed options ---
static const float allSpeeds[] = { 0.02f, 0.05f, 0.08f, 0.10f, 0.15f, 0.25f, 0.40f };
static const int numSpeeds = 7;

static int findSpeedIndex(float speed)
{
    int closest = 0;
    float diff = 999.f;
    for (int i = 0; i < numSpeeds; i++) {
        float d = std::abs(allSpeeds[i] - speed);
        if (d < diff) { diff = d; closest = i; }
    }
    return closest;
}

static std::string speedLabel(float speed)
{
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%.2fs", speed);
    return buf;
}

// --- Init mode / cell size options ---
static const InitMode allInitModes[] = {
    InitMode::RANDOM_50, InitMode::RANDOM_25, InitMode::RANDOM_10,
    InitMode::RANDOM_5, InitMode::SINGLE_SEED, InitMode::CLUSTER
};
static const int numInitModes = 6;

static const int allCellSizes[] = { 1, 2, 3, 5, 10, 15, 20 };
static const int numCellSizes = 7;

// --- Layout constants ---
static const int rowY0 = 88;
static const int rowSpacing = 26;
static const int btnH = 20;
static const int btnGap = 5;
static const int listStartY = rowY0 + rowSpacing * 3 + 8;
static const int itemH = 22;

// --- Button position helpers ---
struct BtnLayout { float x[12]; float w[12]; int count; float labelX; };

static BtnLayout layoutButtons(sf::Font& font, const char* labels[], int n, int fontSize = 11)
{
    BtnLayout bl;
    bl.count = n;
    float totalW = 0;
    for (int i = 0; i < n; i++) {
        sf::Text t(labels[i], font, fontSize);
        bl.w[i] = t.getGlobalBounds().width + 14;
        totalW += bl.w[i] + btnGap;
    }
    totalW -= btnGap;
    float x = (windowWidth - totalW) / 2.f;
    bl.labelX = x;
    for (int i = 0; i < n; i++) {
        bl.x[i] = x;
        x += bl.w[i] + btnGap;
    }
    return bl;
}

// --- Draw a row of toggle buttons ---
static void drawButtonRow(sf::RenderWindow& window, sf::Font& font,
    int y, const char* rowLabel, const char* labels[], int n, int activeIdx,
    sf::Color activeCol, sf::Color activeBorder)
{
    sf::Text lbl(rowLabel, font, 10);
    lbl.setFillColor(sf::Color(100, 100, 120));
    float totalW = 0;
    for (int i = 0; i < n; i++) {
        sf::Text t(labels[i], font, 11);
        totalW += t.getGlobalBounds().width + 14 + btnGap;
    }
    totalW -= btnGap;
    float startX = (windowWidth - totalW) / 2.f;
    lbl.setPosition(startX - lbl.getGlobalBounds().width - 8, y + 3);
    window.draw(lbl);

    float x = startX;
    for (int i = 0; i < n; i++) {
        sf::Text t(labels[i], font, 11);
        float w = t.getGlobalBounds().width + 14;
        bool sel = (i == activeIdx);

        sf::RectangleShape btn(sf::Vector2f(w, btnH));
        btn.setPosition(x, y);
        btn.setFillColor(sel ? activeCol : sf::Color(32, 32, 42));
        btn.setOutlineColor(sel ? activeBorder : sf::Color(55, 55, 65));
        btn.setOutlineThickness(1);
        window.draw(btn);

        t.setFillColor(sel ? sf::Color(220, 230, 255) : sf::Color(120, 120, 135));
        t.setPosition(x + 7, y + 2);
        window.draw(t);

        x += w + btnGap;
    }
}

// Helper: handle clicks on a button row, returns clicked index or -1
static int clickButtonRow(sf::Font& font, int y, const char* labels[], int n, int mx, int my)
{
    if (my < y || my > y + btnH) return -1;
    BtnLayout bl = layoutButtons(font, labels, n);
    for (int i = 0; i < n; i++)
        if (mx >= bl.x[i] && mx <= bl.x[i] + bl.w[i])
            return i;
    return -1;
}

// Draw all 3 config rows and return active indices
static void drawConfigRows(sf::RenderWindow& window, sf::Font& font, int baseY,
    InitMode initMode, int activeCellSize, float activeSpeed)
{
    const char* initLabels[] = { "50%", "25%", "10%", "5%", "Seed", "Cluster" };
    int activeInit = 0;
    for (int i = 0; i < numInitModes; i++)
        if (allInitModes[i] == initMode) { activeInit = i; break; }
    drawButtonRow(window, font, baseY, "Density:", initLabels, numInitModes, activeInit,
        sf::Color(35, 55, 90), sf::Color(70, 110, 190));

    const char* cellLabels[] = { "1px", "2px", "3px", "5px", "10px", "15px", "20px" };
    int activeCell = 0;
    for (int i = 0; i < numCellSizes; i++)
        if (allCellSizes[i] == activeCellSize) { activeCell = i; break; }
    drawButtonRow(window, font, baseY + rowSpacing, "Cell:", cellLabels, numCellSizes, activeCell,
        sf::Color(55, 35, 85), sf::Color(110, 70, 180));

    const char* speedLabels[] = { "0.02s", "0.05s", "0.08s", "0.10s", "0.15s", "0.25s", "0.40s" };
    int activeSpd = findSpeedIndex(activeSpeed);
    drawButtonRow(window, font, baseY + rowSpacing * 2, "Speed:", speedLabels, numSpeeds, activeSpd,
        sf::Color(85, 55, 35), sf::Color(180, 110, 70));
}

// Handle clicks on config rows, returns true if something changed
static bool handleConfigRowClicks(sf::Font& font, int baseY, int mx, int my,
    InitMode& initMode, float& speed)
{
    const char* initLabels[] = { "50%", "25%", "10%", "5%", "Seed", "Cluster" };
    int ci = clickButtonRow(font, baseY, initLabels, numInitModes, mx, my);
    if (ci >= 0) { initMode = allInitModes[ci]; return true; }

    const char* cellLabels[] = { "1px", "2px", "3px", "5px", "10px", "15px", "20px" };
    ci = clickButtonRow(font, baseY + rowSpacing, cellLabels, numCellSizes, mx, my);
    if (ci >= 0) { cellSize = allCellSizes[ci]; return true; }

    const char* speedLabels[] = { "0.02s", "0.05s", "0.08s", "0.10s", "0.15s", "0.25s", "0.40s" };
    ci = clickButtonRow(font, baseY + rowSpacing * 2, speedLabels, numSpeeds, mx, my);
    if (ci >= 0) { speed = allSpeeds[ci]; return true; }

    return false;
}

// ============================================================

Menu::Menu(Grid& grid, RuleSet& rules) : grid(grid), rules(rules),
    currentPage(MenuPage::RULES), selectedIndex(0), scrollOffset(0),
    initMode(InitMode::RANDOM_50), lastWasCustom(false)
{
    font.loadFromFile("resources/arial.ttf");
    presets = RuleSet::getPresets();
    loadSavedConfigs();
    buildVisualItems();
    for (int i = 0; i < (int)visualItems.size(); i++) {
        if (visualItems[i].type != VisualItem::HEADER) { selectedIndex = i; break; }
    }
    std::memset(customBirth, 0, sizeof(customBirth));
    std::memset(customSurvival, 0, sizeof(customSurvival));
    customBirth[3] = true;
    customSurvival[2] = true;
    customSurvival[3] = true;
    shown = false;
}

void Menu::buildVisualItems()
{
    visualItems.clear();

    // Saved configs section
    if (!savedConfigs.empty()) {
        VisualItem h;
        h.type = VisualItem::HEADER;
        h.presetIndex = -1;
        h.label = "Saved";
        visualItems.push_back(h);
        for (int i = 0; i < (int)savedConfigs.size(); i++) {
            VisualItem s;
            s.type = VisualItem::SAVED;
            s.presetIndex = i;
            s.label = savedConfigs[i].name;
            visualItems.push_back(s);
        }
    }

    // Preset categories
    std::string lastCat;
    for (int i = 0; i < (int)presets.size(); i++) {
        if (presets[i].category != lastCat) {
            lastCat = presets[i].category;
            VisualItem h;
            h.type = VisualItem::HEADER;
            h.presetIndex = -1;
            h.label = lastCat;
            visualItems.push_back(h);
        }
        VisualItem p;
        p.type = VisualItem::PRESET;
        p.presetIndex = i;
        p.label = presets[i].name;
        visualItems.push_back(p);
    }
    VisualItem c;
    c.type = VisualItem::CUSTOM_OPT;
    c.presetIndex = -1;
    c.label = "Custom Rules...";
    visualItems.push_back(c);
}

bool Menu::isShown() { return shown; }

void Menu::toggle()
{
    shown = !shown;
    if (shown) {
        currentPage = lastWasCustom ? MenuPage::CUSTOM : MenuPage::RULES;
    }
}

void Menu::showMenu(sf::RenderWindow& window)
{
    switch (currentPage) {
        case MenuPage::RULES:  showRulesPage(window); break;
        case MenuPage::CUSTOM: showCustomPage(window); break;
    }
}

void Menu::handleMenuInput(sf::RenderWindow& window)
{
    switch (currentPage) {
        case MenuPage::RULES:  handleRulesInput(window); break;
        case MenuPage::CUSTOM: handleCustomInput(window); break;
    }
}

const char* Menu::initModeName(InitMode mode)
{
    switch (mode) {
        case InitMode::RANDOM_50:   return "50%";
        case InitMode::RANDOM_25:   return "25%";
        case InitMode::RANDOM_10:   return "10%";
        case InitMode::RANDOM_5:    return "5%";
        case InitMode::SINGLE_SEED: return "Seed";
        case InitMode::CLUSTER:     return "Cluster";
    }
    return "";
}

InitMode Menu::initModeFromInt(int v)
{
    switch (v) {
        case 0: return InitMode::RANDOM_50;
        case 1: return InitMode::RANDOM_25;
        case 2: return InitMode::RANDOM_10;
        case 3: return InitMode::RANDOM_5;
        case 4: return InitMode::SINGLE_SEED;
        case 5: return InitMode::CLUSTER;
    }
    return InitMode::RANDOM_50;
}

static int initModeToInt(InitMode m)
{
    switch (m) {
        case InitMode::RANDOM_50:   return 0;
        case InitMode::RANDOM_25:   return 1;
        case InitMode::RANDOM_10:   return 2;
        case InitMode::RANDOM_5:    return 3;
        case InitMode::SINGLE_SEED: return 4;
        case InitMode::CLUSTER:     return 5;
    }
    return 0;
}

void Menu::moveSelection(int dir)
{
    int n = (int)visualItems.size();
    for (int attempt = 0; attempt < n; attempt++) {
        selectedIndex += dir;
        if (selectedIndex < 0) selectedIndex = n - 1;
        if (selectedIndex >= n) selectedIndex = 0;
        if (visualItems[selectedIndex].type != VisualItem::HEADER) break;
    }
}

void Menu::initializeGrid()
{
    grid.resize(windowWidth / cellSize, windowHeight / cellSize);
    switch (initMode) {
        case InitMode::RANDOM_50:   grid.initializeCellsWithDensity(50); break;
        case InitMode::RANDOM_25:   grid.initializeCellsWithDensity(25); break;
        case InitMode::RANDOM_10:   grid.initializeCellsWithDensity(10); break;
        case InitMode::RANDOM_5:    grid.initializeCellsWithDensity(5);  break;
        case InitMode::SINGLE_SEED: grid.initializeCellsSeed();          break;
        case InitMode::CLUSTER:     grid.initializeCellsCluster();       break;
    }
}

void Menu::selectPreset(int index)
{
    if (index >= 0 && index < (int)presets.size()) {
        rules = presets[index];
        lastWasCustom = false;
        initializeGrid();
        simulationClock.restart();
        paused = false;
        shown = false;
    }
}

void Menu::selectSaved(int index)
{
    if (index >= 0 && index < (int)savedConfigs.size()) {
        const SavedConfig& sc = savedConfigs[index];
        std::vector<int> b, s;
        for (int i = 0; i <= 8; i++) {
            if (sc.birth[i]) b.push_back(i);
            if (sc.survival[i]) s.push_back(i);
        }
        rules.setRule(sc.name, b, s);
        rules.category = "Saved";
        cellSize = sc.cellSz;
        simulationSpeed = sc.speed;
        initMode = sc.init;
        // Also update custom toggles to match
        std::memcpy(customBirth, sc.birth, sizeof(customBirth));
        std::memcpy(customSurvival, sc.survival, sizeof(customSurvival));
        lastWasCustom = false;
        initializeGrid();
        simulationClock.restart();
        paused = false;
        shown = false;
    }
}

void Menu::applyCustomRule()
{
    std::vector<int> birth, survival;
    for (int i = 0; i <= 8; i++) {
        if (customBirth[i]) birth.push_back(i);
        if (customSurvival[i]) survival.push_back(i);
    }
    rules.setRule("Custom", birth, survival);
    rules.category = "";
    lastWasCustom = true;
    initializeGrid();
    simulationClock.restart();
    paused = false;
    shown = false;
}

// --- Save/Load configs ---
static const char* SAVE_FILE = "saved_configs.txt";

void Menu::loadSavedConfigs()
{
    savedConfigs.clear();
    std::ifstream f(SAVE_FILE);
    if (!f.is_open()) return;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        SavedConfig sc;
        std::memset(sc.birth, 0, sizeof(sc.birth));
        std::memset(sc.survival, 0, sizeof(sc.survival));

        // Format: name|b0b1b2..b8|s0s1..s8|cellSz|speed|initMode
        std::string name, bstr, sstr;
        std::getline(ss, name, '|');
        std::getline(ss, bstr, '|');
        std::getline(ss, sstr, '|');
        sc.name = name;
        for (int i = 0; i < 9 && i < (int)bstr.size(); i++)
            sc.birth[i] = (bstr[i] == '1');
        for (int i = 0; i < 9 && i < (int)sstr.size(); i++)
            sc.survival[i] = (sstr[i] == '1');

        std::string tok;
        if (std::getline(ss, tok, '|')) sc.cellSz = std::stoi(tok);
        else sc.cellSz = 10;
        if (std::getline(ss, tok, '|')) sc.speed = std::stof(tok);
        else sc.speed = 0.15f;
        if (std::getline(ss, tok, '|')) sc.init = initModeFromInt(std::stoi(tok));
        else sc.init = InitMode::RANDOM_50;

        savedConfigs.push_back(sc);
    }
}

void Menu::writeSavedConfigs()
{
    std::ofstream f(SAVE_FILE);
    if (!f.is_open()) return;
    for (const auto& sc : savedConfigs) {
        f << sc.name << '|';
        for (int i = 0; i < 9; i++) f << (sc.birth[i] ? '1' : '0');
        f << '|';
        for (int i = 0; i < 9; i++) f << (sc.survival[i] ? '1' : '0');
        f << '|' << sc.cellSz << '|' << sc.speed << '|' << initModeToInt(sc.init) << '\n';
    }
}

void Menu::saveCurrentConfig()
{
    SavedConfig sc;
    // Build notation for the name
    std::string notation = rules.getNotation();
    sc.name = rules.name;
    if (sc.name == "Custom") sc.name = notation;

    // Check for duplicate name and make unique
    int dup = 1;
    std::string baseName = sc.name;
    for (bool found = true; found;) {
        found = false;
        for (const auto& existing : savedConfigs) {
            if (existing.name == sc.name) {
                found = true;
                sc.name = baseName + " (" + std::to_string(++dup) + ")";
                break;
            }
        }
    }

    std::memcpy(sc.birth, rules.birthConditions, sizeof(sc.birth));
    std::memcpy(sc.survival, rules.survivalConditions, sizeof(sc.survival));
    sc.cellSz = cellSize;
    sc.speed = simulationSpeed;
    sc.init = initMode;

    savedConfigs.push_back(sc);
    writeSavedConfigs();
    buildVisualItems();
    // Fix selectedIndex if needed
    if (selectedIndex >= (int)visualItems.size())
        selectedIndex = (int)visualItems.size() - 1;
}

// ============================================================
// Rules Page
// ============================================================

void Menu::showRulesPage(sf::RenderWindow& window)
{
    window.clear(sf::Color(18, 18, 28));

    // --- Title ---
    sf::Text title("CELLULAR AUTOMATA", font, 28);
    title.setFillColor(sf::Color(220, 220, 240));
    title.setStyle(sf::Text::Bold);
    title.setPosition(windowWidth / 2.f - title.getGlobalBounds().width / 2.f, 8);
    window.draw(title);

    sf::RectangleShape sep(sf::Vector2f(windowWidth - 100, 1));
    sep.setPosition(50, 42);
    sep.setFillColor(sf::Color(50, 50, 70));
    window.draw(sep);

    // Active rule + grid info
    std::string activeStr = rules.name + "  " + rules.getNotation()
        + "   |   " + std::to_string(windowWidth / cellSize) + "x"
        + std::to_string(windowHeight / cellSize) + " grid"
        + "   |   " + speedLabel(simulationSpeed);
    sf::Text info(activeStr, font, 11);
    info.setFillColor(sf::Color(80, 180, 100));
    info.setPosition(windowWidth / 2.f - info.getGlobalBounds().width / 2.f, 50);
    window.draw(info);

    sf::Text subtitle("Configure and select a ruleset", font, 11);
    subtitle.setFillColor(sf::Color(120, 120, 145));
    subtitle.setPosition(windowWidth / 2.f - subtitle.getGlobalBounds().width / 2.f, 70);
    window.draw(subtitle);

    // --- Config button rows ---
    drawConfigRows(window, font, rowY0, initMode, cellSize, simulationSpeed);

    // --- Separator before list ---
    sf::RectangleShape sep2(sf::Vector2f(windowWidth - 60, 1));
    sep2.setPosition(30, listStartY - 6);
    sep2.setFillColor(sf::Color(45, 45, 60));
    window.draw(sep2);

    // --- Rule list ---
    int totalVisual = (int)visualItems.size();
    int maxVisible = (windowHeight - listStartY - 28) / itemH;

    if (scrollOffset > totalVisual - maxVisible) scrollOffset = totalVisual - maxVisible;
    if (scrollOffset < 0) scrollOffset = 0;

    for (int vi = 0; vi < maxVisible && (vi + scrollOffset) < totalVisual; vi++)
    {
        int idx = vi + scrollOffset;
        const VisualItem& item = visualItems[idx];
        float yPos = listStartY + vi * itemH;

        if (item.type == VisualItem::HEADER)
        {
            sf::Text hdr(item.label, font, 10);
            hdr.setFillColor(sf::Color(100, 130, 180));
            hdr.setStyle(sf::Text::Bold);
            hdr.setPosition(30, yPos + 4);
            window.draw(hdr);

            sf::RectangleShape hline(sf::Vector2f(windowWidth - 60 - hdr.getGlobalBounds().width - 12, 1));
            hline.setPosition(38 + hdr.getGlobalBounds().width + 8, yPos + 12);
            hline.setFillColor(sf::Color(45, 55, 75));
            window.draw(hline);
        }
        else
        {
            if (idx == selectedIndex) {
                sf::RectangleShape hl(sf::Vector2f(windowWidth - 50, itemH - 2));
                hl.setPosition(25, yPos);
                hl.setFillColor(sf::Color(35, 35, 70));
                hl.setOutlineColor(sf::Color(65, 65, 130));
                hl.setOutlineThickness(1);
                window.draw(hl);
            }

            if (item.type == VisualItem::PRESET) {
                sf::Text nameText(item.label, font, 13);
                nameText.setPosition(40, yPos + 2);
                nameText.setFillColor(idx == selectedIndex ? sf::Color(255, 240, 140) : sf::Color(200, 200, 215));
                window.draw(nameText);

                sf::Text notation(presets[item.presetIndex].getNotation(), font, 11);
                notation.setFillColor(sf::Color(85, 160, 110));
                notation.setPosition(windowWidth - 45 - notation.getGlobalBounds().width, yPos + 3);
                window.draw(notation);
            }
            else if (item.type == VisualItem::SAVED) {
                sf::Text nameText(item.label, font, 13);
                nameText.setPosition(40, yPos + 2);
                nameText.setFillColor(idx == selectedIndex ? sf::Color(255, 240, 140) : sf::Color(180, 210, 255));
                window.draw(nameText);

                // Show config summary on right
                const SavedConfig& sc = savedConfigs[item.presetIndex];
                std::string summ = std::to_string(sc.cellSz) + "px " + speedLabel(sc.speed);
                sf::Text summText(summ, font, 10);
                summText.setFillColor(sf::Color(100, 140, 180));
                summText.setPosition(windowWidth - 45 - summText.getGlobalBounds().width, yPos + 4);
                window.draw(summText);

                // Delete X button
                sf::Text delX("x", font, 10);
                delX.setFillColor(sf::Color(120, 70, 70));
                delX.setPosition(windowWidth - 30, yPos + 4);
                window.draw(delX);
            }
            else {
                sf::Text ct(item.label, font, 13);
                ct.setPosition(40, yPos + 2);
                ct.setFillColor(idx == selectedIndex ? sf::Color(255, 240, 140) : sf::Color(160, 160, 230));
                ct.setStyle(sf::Text::Italic);
                window.draw(ct);
            }
        }
    }

    // Scroll indicators
    if (scrollOffset > 0) {
        sf::Text up("^", font, 12);
        up.setFillColor(sf::Color(120, 120, 140));
        up.setPosition(windowWidth - 22, listStartY);
        window.draw(up);
    }
    if (scrollOffset + maxVisible < totalVisual) {
        sf::Text dn("v", font, 12);
        dn.setFillColor(sf::Color(120, 120, 140));
        dn.setPosition(windowWidth - 22, listStartY + (maxVisible - 1) * itemH);
        window.draw(dn);
    }

    sf::Text hint("Up/Down + Enter  |  Click to select  |  +/- speed  |  Esc quit", font, 10);
    hint.setFillColor(sf::Color(70, 70, 90));
    hint.setPosition(windowWidth / 2.f - hint.getGlobalBounds().width / 2.f, windowHeight - 20);
    window.draw(hint);

    window.display();
}

void Menu::handleRulesInput(sf::RenderWindow& window)
{
    int totalVisual = (int)visualItems.size();
    int maxVisible = (windowHeight - listStartY - 28) / itemH;

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) { window.close(); return; }
        if (event.type == sf::Event::Resized)
            window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                window.close();
            else if (event.key.code == sf::Keyboard::M) {
                toggle(); paused = isShown();
            }
            else if (event.key.code == sf::Keyboard::Up) {
                moveSelection(-1);
                if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
                if (selectedIndex >= scrollOffset + maxVisible) scrollOffset = selectedIndex - maxVisible + 1;
                while (scrollOffset > 0 && visualItems[scrollOffset].type == VisualItem::HEADER
                       && scrollOffset > selectedIndex - 1)
                    scrollOffset--;
            }
            else if (event.key.code == sf::Keyboard::Down) {
                moveSelection(1);
                if (selectedIndex >= scrollOffset + maxVisible) scrollOffset = selectedIndex - maxVisible + 1;
                if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
            }
            else if (event.key.code == sf::Keyboard::Return) {
                const auto& item = visualItems[selectedIndex];
                if (item.type == VisualItem::PRESET) selectPreset(item.presetIndex);
                else if (item.type == VisualItem::SAVED) selectSaved(item.presetIndex);
                else if (item.type == VisualItem::CUSTOM_OPT) currentPage = MenuPage::CUSTOM;
            }
            else if (event.key.code == sf::Keyboard::Equal) {
                int i = findSpeedIndex(simulationSpeed);
                if (i > 0) simulationSpeed = allSpeeds[i - 1];
            }
            else if (event.key.code == sf::Keyboard::Hyphen) {
                int i = findSpeedIndex(simulationSpeed);
                if (i < numSpeeds - 1) simulationSpeed = allSpeeds[i + 1];
            }
            else if (event.key.code == sf::Keyboard::Delete || event.key.code == sf::Keyboard::BackSpace) {
                // Delete saved config if selected
                const auto& item = visualItems[selectedIndex];
                if (item.type == VisualItem::SAVED) {
                    savedConfigs.erase(savedConfigs.begin() + item.presetIndex);
                    writeSavedConfigs();
                    buildVisualItems();
                    if (selectedIndex >= (int)visualItems.size())
                        selectedIndex = (int)visualItems.size() - 1;
                    if (visualItems[selectedIndex].type == VisualItem::HEADER)
                        moveSelection(1);
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f pos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            int mx = (int)pos.x, my = (int)pos.y;

            // Config row clicks
            handleConfigRowClicks(font, rowY0, mx, my, initMode, simulationSpeed);

            // Rule list clicks
            if (my >= listStartY && my < listStartY + maxVisible * itemH) {
                int clickedVi = (my - listStartY) / itemH + scrollOffset;
                if (clickedVi >= 0 && clickedVi < totalVisual) {
                    const auto& item = visualItems[clickedVi];
                    if (item.type == VisualItem::PRESET) {
                        selectedIndex = clickedVi;
                        selectPreset(item.presetIndex);
                    } else if (item.type == VisualItem::SAVED) {
                        // Check if click is on the delete X
                        if (mx >= windowWidth - 38) {
                            savedConfigs.erase(savedConfigs.begin() + item.presetIndex);
                            writeSavedConfigs();
                            buildVisualItems();
                            if (selectedIndex >= (int)visualItems.size())
                                selectedIndex = (int)visualItems.size() - 1;
                            if (selectedIndex >= 0 && visualItems[selectedIndex].type == VisualItem::HEADER)
                                moveSelection(1);
                        } else {
                            selectedIndex = clickedVi;
                            selectSaved(item.presetIndex);
                        }
                    } else if (item.type == VisualItem::CUSTOM_OPT) {
                        selectedIndex = clickedVi;
                        currentPage = MenuPage::CUSTOM;
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            scrollOffset -= (int)event.mouseWheelScroll.delta * 2;
            if (scrollOffset < 0) scrollOffset = 0;
            if (scrollOffset > totalVisual - maxVisible) scrollOffset = totalVisual - maxVisible;
            if (scrollOffset < 0) scrollOffset = 0;
        }
    }
}

// ============================================================
// Custom Rules Page
// ============================================================

void Menu::showCustomPage(sf::RenderWindow& window)
{
    window.clear(sf::Color(18, 18, 28));

    sf::Text title("CUSTOM RULES", font, 26);
    title.setFillColor(sf::Color(220, 220, 240));
    title.setStyle(sf::Text::Bold);
    title.setPosition(windowWidth / 2.f - title.getGlobalBounds().width / 2.f, 12);
    window.draw(title);

    sf::RectangleShape sep(sf::Vector2f(windowWidth - 100, 1));
    sep.setPosition(50, 42);
    sep.setFillColor(sf::Color(50, 50, 70));
    window.draw(sep);

    // Current notation
    std::string notation = "B";
    for (int i = 0; i <= 8; i++) if (customBirth[i]) notation += std::to_string(i);
    notation += " / S";
    for (int i = 0; i <= 8; i++) if (customSurvival[i]) notation += std::to_string(i);

    sf::Text notationText(notation, font, 20);
    notationText.setFillColor(sf::Color(90, 210, 100));
    notationText.setPosition(windowWidth / 2.f - notationText.getGlobalBounds().width / 2.f, 52);
    window.draw(notationText);

    // Descriptions
    sf::Text desc("Birth: neighbor count that makes a dead cell come alive", font, 10);
    desc.setFillColor(sf::Color(130, 130, 150));
    desc.setPosition(windowWidth / 2.f - desc.getGlobalBounds().width / 2.f, 82);
    window.draw(desc);

    sf::Text desc2("Survival: neighbor count that lets a living cell survive", font, 10);
    desc2.setFillColor(sf::Color(130, 130, 150));
    desc2.setPosition(windowWidth / 2.f - desc2.getGlobalBounds().width / 2.f, 95);
    window.draw(desc2);

    // B/S toggle grid - more compact
    float numStartX = 280;
    float numSpacing = 44;
    int boxSize = 30;

    // Birth row
    sf::Text birthLabel("Birth:", font, 15);
    birthLabel.setFillColor(sf::Color(180, 200, 220));
    birthLabel.setPosition(130, 125);
    window.draw(birthLabel);

    for (int i = 0; i <= 8; i++) {
        float xPos = numStartX + i * numSpacing;
        sf::RectangleShape box(sf::Vector2f(boxSize, boxSize));
        box.setPosition(xPos - 4, 122);
        box.setFillColor(customBirth[i] ? sf::Color(25, 90, 25) : sf::Color(40, 40, 50));
        box.setOutlineColor(customBirth[i] ? sf::Color(50, 180, 50) : sf::Color(65, 65, 75));
        box.setOutlineThickness(1);
        window.draw(box);

        sf::Text num(std::to_string(i), font, 15);
        num.setFillColor(customBirth[i] ? sf::Color(90, 240, 90) : sf::Color(90, 90, 100));
        num.setPosition(xPos + 5, 125);
        window.draw(num);
    }

    // Survival row
    sf::Text survLabel("Survival:", font, 15);
    survLabel.setFillColor(sf::Color(180, 200, 220));
    survLabel.setPosition(130, 168);
    window.draw(survLabel);

    for (int i = 0; i <= 8; i++) {
        float xPos = numStartX + i * numSpacing;
        sf::RectangleShape box(sf::Vector2f(boxSize, boxSize));
        box.setPosition(xPos - 4, 165);
        box.setFillColor(customSurvival[i] ? sf::Color(25, 25, 90) : sf::Color(40, 40, 50));
        box.setOutlineColor(customSurvival[i] ? sf::Color(50, 50, 180) : sf::Color(65, 65, 75));
        box.setOutlineThickness(1);
        window.draw(box);

        sf::Text num(std::to_string(i), font, 15);
        num.setFillColor(customSurvival[i] ? sf::Color(90, 90, 240) : sf::Color(90, 90, 100));
        num.setPosition(xPos + 5, 168);
        window.draw(num);
    }

    // Separator
    sf::RectangleShape sep2(sf::Vector2f(windowWidth - 100, 1));
    sep2.setPosition(50, 210);
    sep2.setFillColor(sf::Color(45, 45, 60));
    window.draw(sep2);

    // Config button rows
    int configBaseY = 222;
    drawConfigRows(window, font, configBaseY, initMode, cellSize, simulationSpeed);

    // Grid info
    std::string gridInfo = std::to_string(windowWidth / cellSize) + "x"
        + std::to_string(windowHeight / cellSize) + " grid  ("
        + std::to_string((windowWidth / cellSize) * (windowHeight / cellSize)) + " cells)";
    sf::Text gridText(gridInfo, font, 10);
    gridText.setFillColor(sf::Color(100, 140, 180));
    gridText.setPosition(windowWidth / 2.f - gridText.getGlobalBounds().width / 2.f,
        configBaseY + rowSpacing * 3 + 2);
    window.draw(gridText);

    // Buttons row
    float btnRowY = configBaseY + rowSpacing * 3 + 22;

    // Apply & Start button
    float applyW = 180, applyH = 32;
    sf::RectangleShape applyBtn(sf::Vector2f(applyW, applyH));
    applyBtn.setPosition(windowWidth / 2.f - applyW - 8, btnRowY);
    applyBtn.setFillColor(sf::Color(25, 70, 25));
    applyBtn.setOutlineColor(sf::Color(50, 140, 50));
    applyBtn.setOutlineThickness(1);
    window.draw(applyBtn);
    sf::Text applyText("Apply & Start", font, 14);
    applyText.setFillColor(sf::Color(90, 240, 90));
    applyText.setPosition(windowWidth / 2.f - applyW + applyW / 2.f
        - applyText.getGlobalBounds().width / 2.f - 8, btnRowY + 7);
    window.draw(applyText);

    // Back button
    sf::RectangleShape backBtn(sf::Vector2f(applyW, applyH));
    backBtn.setPosition(windowWidth / 2.f + 8, btnRowY);
    backBtn.setFillColor(sf::Color(50, 40, 40));
    backBtn.setOutlineColor(sf::Color(120, 85, 85));
    backBtn.setOutlineThickness(1);
    window.draw(backBtn);
    sf::Text backText("Back to Presets", font, 14);
    backText.setFillColor(sf::Color(200, 160, 160));
    backText.setPosition(windowWidth / 2.f + 8 + applyW / 2.f
        - backText.getGlobalBounds().width / 2.f, btnRowY + 7);
    window.draw(backText);

    sf::Text hint("Click numbers to toggle  |  Esc = back  |  Enter = apply", font, 10);
    hint.setFillColor(sf::Color(70, 70, 90));
    hint.setPosition(windowWidth / 2.f - hint.getGlobalBounds().width / 2.f, windowHeight - 20);
    window.draw(hint);

    window.display();
}

void Menu::handleCustomInput(sf::RenderWindow& window)
{
    float numStartX = 280;
    float numSpacing = 44;
    int boxSize = 30;
    int configBaseY = 222;
    float btnRowY = configBaseY + rowSpacing * 3 + 22;
    float applyW = 180, applyH = 32;

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) { window.close(); return; }
        if (event.type == sf::Event::Resized)
            window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) currentPage = MenuPage::RULES;
            else if (event.key.code == sf::Keyboard::M) { toggle(); paused = isShown(); }
            else if (event.key.code == sf::Keyboard::Return) applyCustomRule();
            else if (event.key.code == sf::Keyboard::Equal) {
                int i = findSpeedIndex(simulationSpeed);
                if (i > 0) simulationSpeed = allSpeeds[i - 1];
            }
            else if (event.key.code == sf::Keyboard::Hyphen) {
                int i = findSpeedIndex(simulationSpeed);
                if (i < numSpeeds - 1) simulationSpeed = allSpeeds[i + 1];
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f pos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            int mx = (int)pos.x, my = (int)pos.y;

            // Birth toggles
            for (int i = 0; i <= 8; i++) {
                float xPos = numStartX + i * numSpacing - 4;
                if (mx >= xPos && mx <= xPos + boxSize && my >= 122 && my <= 122 + boxSize)
                    customBirth[i] = !customBirth[i];
            }

            // Survival toggles
            for (int i = 0; i <= 8; i++) {
                float xPos = numStartX + i * numSpacing - 4;
                if (mx >= xPos && mx <= xPos + boxSize && my >= 165 && my <= 165 + boxSize)
                    customSurvival[i] = !customSurvival[i];
            }

            // Config row clicks
            handleConfigRowClicks(font, configBaseY, mx, my, initMode, simulationSpeed);

            // Apply button
            float applyX = windowWidth / 2.f - applyW - 8;
            if (mx >= applyX && mx <= applyX + applyW && my >= btnRowY && my <= btnRowY + applyH)
                applyCustomRule();

            // Back button
            float backX = windowWidth / 2.f + 8;
            if (mx >= backX && mx <= backX + applyW && my >= btnRowY && my <= btnRowY + applyH)
                currentPage = MenuPage::RULES;
        }
    }
}
