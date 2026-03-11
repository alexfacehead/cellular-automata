#include "Menu.h"
#include <cstring>
#include <cstdio>

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
static const int rowY0 = 88;     // first button row
static const int rowSpacing = 26;
static const int btnH = 20;
static const int btnGap = 5;
static const int listStartY = rowY0 + rowSpacing * 3 + 8;
static const int itemH = 22;
static const int headerH = 26;

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

// ============================================================

Menu::Menu(Grid& grid, RuleSet& rules) : grid(grid), rules(rules),
    currentPage(MenuPage::RULES), selectedIndex(0), scrollOffset(0),
    initMode(InitMode::RANDOM_50)
{
    font.loadFromFile("resources/arial.ttf");
    presets = RuleSet::getPresets();
    buildVisualItems();
    // Default selectedIndex to first selectable item
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
    if (shown) currentPage = MenuPage::RULES;
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
    initializeGrid();
    simulationClock.restart();
    paused = false;
    shown = false;
}

// --- Draw a row of toggle buttons ---
static void drawButtonRow(sf::RenderWindow& window, sf::Font& font,
    int y, const char* rowLabel, const char* labels[], int n, int activeIdx,
    sf::Color activeCol, sf::Color activeBorder)
{
    // Row label
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

    // Separator
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

    // --- Subtitle ---
    sf::Text subtitle("Configure and select a ruleset", font, 11);
    subtitle.setFillColor(sf::Color(120, 120, 145));
    subtitle.setPosition(windowWidth / 2.f - subtitle.getGlobalBounds().width / 2.f, 70);
    window.draw(subtitle);

    // --- Button rows ---
    // Init mode
    const char* initLabels[] = { "50%", "25%", "10%", "5%", "Seed", "Cluster" };
    int activeInit = 0;
    for (int i = 0; i < numInitModes; i++)
        if (allInitModes[i] == initMode) { activeInit = i; break; }
    drawButtonRow(window, font, rowY0, "Density:", initLabels, numInitModes, activeInit,
        sf::Color(35, 55, 90), sf::Color(70, 110, 190));

    // Cell size
    const char* cellLabels[] = { "1px", "2px", "3px", "5px", "10px", "15px", "20px" };
    int activeCell = 0;
    for (int i = 0; i < numCellSizes; i++)
        if (allCellSizes[i] == cellSize) { activeCell = i; break; }
    drawButtonRow(window, font, rowY0 + rowSpacing, "Cell:", cellLabels, numCellSizes, activeCell,
        sf::Color(55, 35, 85), sf::Color(110, 70, 180));

    // Speed
    const char* speedLabels[] = { "0.02s", "0.05s", "0.08s", "0.10s", "0.15s", "0.25s", "0.40s" };
    int activeSpeed = findSpeedIndex(simulationSpeed);
    drawButtonRow(window, font, rowY0 + rowSpacing * 2, "Speed:", speedLabels, numSpeeds, activeSpeed,
        sf::Color(85, 55, 35), sf::Color(180, 110, 70));

    // --- Separator before list ---
    sf::RectangleShape sep2(sf::Vector2f(windowWidth - 60, 1));
    sep2.setPosition(30, listStartY - 6);
    sep2.setFillColor(sf::Color(45, 45, 60));
    window.draw(sep2);

    // --- Rule list with category headers ---
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
            // Category header
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
            // Highlight
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
            else {
                // Custom option
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

    // Hint
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
                // Also scroll past any headers above
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
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f pos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            int mx = (int)pos.x, my = (int)pos.y;

            // --- Button row clicks ---
            // Recompute button positions for each row
            auto checkRow = [&](int y, int n, auto callback) {
                if (my < y || my > y + btnH) return;
                // Measure buttons to find positions
                float totalW = 0;
                float ws[12];
                for (int i = 0; i < n; i++) {
                    // We need the actual labels... handled per-row below
                }
                (void)totalW; (void)ws;
                callback(mx, my);
            };
            (void)checkRow;

            // Init mode row
            {
                const char* labels[] = { "50%", "25%", "10%", "5%", "Seed", "Cluster" };
                BtnLayout bl = layoutButtons(font, labels, numInitModes);
                int y = rowY0;
                if (my >= y && my <= y + btnH) {
                    for (int i = 0; i < numInitModes; i++)
                        if (mx >= bl.x[i] && mx <= bl.x[i] + bl.w[i])
                            initMode = allInitModes[i];
                }
            }

            // Cell size row
            {
                const char* labels[] = { "1px", "2px", "3px", "5px", "10px", "15px", "20px" };
                BtnLayout bl = layoutButtons(font, labels, numCellSizes);
                int y = rowY0 + rowSpacing;
                if (my >= y && my <= y + btnH) {
                    for (int i = 0; i < numCellSizes; i++)
                        if (mx >= bl.x[i] && mx <= bl.x[i] + bl.w[i])
                            cellSize = allCellSizes[i];
                }
            }

            // Speed row
            {
                const char* labels[] = { "0.02s", "0.05s", "0.08s", "0.10s", "0.15s", "0.25s", "0.40s" };
                BtnLayout bl = layoutButtons(font, labels, numSpeeds);
                int y = rowY0 + rowSpacing * 2;
                if (my >= y && my <= y + btnH) {
                    for (int i = 0; i < numSpeeds; i++)
                        if (mx >= bl.x[i] && mx <= bl.x[i] + bl.w[i])
                            simulationSpeed = allSpeeds[i];
                }
            }

            // Rule list clicks
            if (my >= listStartY && my < listStartY + maxVisible * itemH) {
                int clickedVi = (my - listStartY) / itemH + scrollOffset;
                if (clickedVi >= 0 && clickedVi < totalVisual) {
                    const auto& item = visualItems[clickedVi];
                    if (item.type == VisualItem::PRESET) {
                        selectedIndex = clickedVi;
                        selectPreset(item.presetIndex);
                    } else if (item.type == VisualItem::CUSTOM_OPT) {
                        selectedIndex = clickedVi;
                        currentPage = MenuPage::CUSTOM;
                    }
                    // HEADER clicks are ignored
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
    title.setPosition(windowWidth / 2.f - title.getGlobalBounds().width / 2.f, 25);
    window.draw(title);

    sf::RectangleShape sep(sf::Vector2f(windowWidth - 100, 1));
    sep.setPosition(50, 58);
    sep.setFillColor(sf::Color(50, 50, 70));
    window.draw(sep);

    // Current notation
    std::string notation = "B";
    for (int i = 0; i <= 8; i++) if (customBirth[i]) notation += std::to_string(i);
    notation += " / S";
    for (int i = 0; i <= 8; i++) if (customSurvival[i]) notation += std::to_string(i);

    sf::Text notationText(notation, font, 22);
    notationText.setFillColor(sf::Color(90, 210, 100));
    notationText.setPosition(windowWidth / 2.f - notationText.getGlobalBounds().width / 2.f, 75);
    window.draw(notationText);

    // Descriptions
    sf::Text desc("Birth: neighbor count that makes a dead cell come alive", font, 11);
    desc.setFillColor(sf::Color(130, 130, 150));
    desc.setPosition(windowWidth / 2.f - desc.getGlobalBounds().width / 2.f, 120);
    window.draw(desc);

    sf::Text desc2("Survival: neighbor count that lets a living cell survive", font, 11);
    desc2.setFillColor(sf::Color(130, 130, 150));
    desc2.setPosition(windowWidth / 2.f - desc2.getGlobalBounds().width / 2.f, 136);
    window.draw(desc2);

    float numStartX = 280;
    float numSpacing = 48;

    // Birth row
    sf::Text birthLabel("Birth:", font, 18);
    birthLabel.setFillColor(sf::Color(180, 200, 220));
    birthLabel.setPosition(110, 185);
    window.draw(birthLabel);

    for (int i = 0; i <= 8; i++) {
        float xPos = numStartX + i * numSpacing;
        sf::RectangleShape box(sf::Vector2f(34, 34));
        box.setPosition(xPos - 4, 182);
        box.setFillColor(customBirth[i] ? sf::Color(25, 90, 25) : sf::Color(40, 40, 50));
        box.setOutlineColor(customBirth[i] ? sf::Color(50, 180, 50) : sf::Color(65, 65, 75));
        box.setOutlineThickness(1);
        window.draw(box);

        sf::Text num(std::to_string(i), font, 18);
        num.setFillColor(customBirth[i] ? sf::Color(90, 240, 90) : sf::Color(90, 90, 100));
        num.setPosition(xPos + 5, 186);
        window.draw(num);
    }

    // Survival row
    sf::Text survLabel("Survival:", font, 18);
    survLabel.setFillColor(sf::Color(180, 200, 220));
    survLabel.setPosition(110, 248);
    window.draw(survLabel);

    for (int i = 0; i <= 8; i++) {
        float xPos = numStartX + i * numSpacing;
        sf::RectangleShape box(sf::Vector2f(34, 34));
        box.setPosition(xPos - 4, 245);
        box.setFillColor(customSurvival[i] ? sf::Color(25, 25, 90) : sf::Color(40, 40, 50));
        box.setOutlineColor(customSurvival[i] ? sf::Color(50, 50, 180) : sf::Color(65, 65, 75));
        box.setOutlineThickness(1);
        window.draw(box);

        sf::Text num(std::to_string(i), font, 18);
        num.setFillColor(customSurvival[i] ? sf::Color(90, 90, 240) : sf::Color(90, 90, 100));
        num.setPosition(xPos + 5, 249);
        window.draw(num);
    }

    // Info line
    std::string infoStr = "Density: " + std::string(initModeName(initMode))
        + "  |  Cell: " + std::to_string(cellSize) + "px"
        + "  |  Grid: " + std::to_string(windowWidth / cellSize) + "x" + std::to_string(windowHeight / cellSize)
        + "  |  Speed: " + speedLabel(simulationSpeed);
    sf::Text infoText(infoStr, font, 11);
    infoText.setFillColor(sf::Color(120, 170, 220));
    infoText.setPosition(windowWidth / 2.f - infoText.getGlobalBounds().width / 2.f, 310);
    window.draw(infoText);

    // Apply button
    sf::RectangleShape applyBtn(sf::Vector2f(200, 38));
    applyBtn.setPosition(windowWidth / 2.f - 100, 350);
    applyBtn.setFillColor(sf::Color(25, 70, 25));
    applyBtn.setOutlineColor(sf::Color(50, 140, 50));
    applyBtn.setOutlineThickness(1);
    window.draw(applyBtn);
    sf::Text applyText("Apply & Start", font, 16);
    applyText.setFillColor(sf::Color(90, 240, 90));
    applyText.setPosition(windowWidth / 2.f - applyText.getGlobalBounds().width / 2.f, 358);
    window.draw(applyText);

    // Back button
    sf::RectangleShape backBtn(sf::Vector2f(200, 38));
    backBtn.setPosition(windowWidth / 2.f - 100, 405);
    backBtn.setFillColor(sf::Color(50, 40, 40));
    backBtn.setOutlineColor(sf::Color(120, 85, 85));
    backBtn.setOutlineThickness(1);
    window.draw(backBtn);
    sf::Text backText("Back", font, 16);
    backText.setFillColor(sf::Color(200, 160, 160));
    backText.setPosition(windowWidth / 2.f - backText.getGlobalBounds().width / 2.f, 413);
    window.draw(backText);

    sf::Text hint("Click numbers to toggle  |  Settings are on the rules page", font, 10);
    hint.setFillColor(sf::Color(70, 70, 90));
    hint.setPosition(windowWidth / 2.f - hint.getGlobalBounds().width / 2.f, windowHeight - 20);
    window.draw(hint);

    window.display();
}

void Menu::handleCustomInput(sf::RenderWindow& window)
{
    float numStartX = 280;
    float numSpacing = 48;

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
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f pos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            int mx = (int)pos.x, my = (int)pos.y;

            // Birth toggles
            for (int i = 0; i <= 8; i++) {
                float xPos = numStartX + i * numSpacing - 4;
                if (mx >= xPos && mx <= xPos + 34 && my >= 182 && my <= 216)
                    customBirth[i] = !customBirth[i];
            }

            // Survival toggles
            for (int i = 0; i <= 8; i++) {
                float xPos = numStartX + i * numSpacing - 4;
                if (mx >= xPos && mx <= xPos + 34 && my >= 245 && my <= 279)
                    customSurvival[i] = !customSurvival[i];
            }

            // Apply
            if (mx >= windowWidth / 2 - 100 && mx <= windowWidth / 2 + 100 && my >= 350 && my <= 388)
                applyCustomRule();
            // Back
            if (mx >= windowWidth / 2 - 100 && mx <= windowWidth / 2 + 100 && my >= 405 && my <= 443)
                currentPage = MenuPage::RULES;
        }
    }
}
