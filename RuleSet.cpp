#include "RuleSet.h"
#include "Grid.h"
#include <cstring>

RuleSet::RuleSet()
{
    name = "Conway's Game of Life";
    category = "Classic";
    std::memset(birthConditions, 0, sizeof(birthConditions));
    std::memset(survivalConditions, 0, sizeof(survivalConditions));
    birthConditions[3] = true;
    survivalConditions[2] = true;
    survivalConditions[3] = true;
}

RuleSet::~RuleSet() {}

void RuleSet::setRule(const std::string& ruleName,
                      const std::vector<int>& birth,
                      const std::vector<int>& survival)
{
    name = ruleName;
    std::memset(birthConditions, 0, sizeof(birthConditions));
    std::memset(survivalConditions, 0, sizeof(survivalConditions));
    for (int b : birth) if (b >= 0 && b <= 8) birthConditions[b] = true;
    for (int s : survival) if (s >= 0 && s <= 8) survivalConditions[s] = true;
}

int RuleSet::countNeighbors(const Grid& grid, int x, int y) const
{
    const int width = grid.getWidth();
    const int height = grid.getHeight();
    int count = 0;

    for (int i = x - 1; i <= x + 1; ++i)
    {
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (i >= 0 && i < width && j >= 0 && j < height && !(i == x && j == y))
            {
                if (grid.getCell(i, j).isAlive()) count++;
            }
        }
    }
    return count;
}

bool RuleSet::applyRuleChosen(const Grid& grid, int x, int y) const
{
    int neighbors = countNeighbors(grid, x, y);
    if (grid.getCell(x, y).isAlive())
        return survivalConditions[neighbors];
    else
        return birthConditions[neighbors];
}

std::string RuleSet::getNotation() const
{
    std::string result = "B";
    for (int i = 0; i <= 8; i++)
        if (birthConditions[i]) result += std::to_string(i);
    result += "/S";
    for (int i = 0; i <= 8; i++)
        if (survivalConditions[i]) result += std::to_string(i);
    return result;
}

std::vector<RuleSet> RuleSet::getPresets()
{
    std::vector<RuleSet> presets;
    auto add = [&](const std::string& cat, const std::string& n,
                    std::vector<int> b, std::vector<int> s) {
        RuleSet r;
        r.setRule(n, b, s);
        r.category = cat;
        presets.push_back(r);
    };

    // Classic
    add("Classic", "Conway's Game of Life",  {3},              {2,3});
    add("Classic", "HighLife",               {3,6},            {2,3});
    add("Classic", "Life 34",                {3,4},            {3,4});
    add("Classic", "Pseudo Life",            {3,5,7},          {2,3,8});
    add("Classic", "Inverse Life",           {0,1,2,3,4,7,8},  {3,4,6,7,8});

    // Chaotic
    add("Chaotic", "Seeds",                  {2},              {});
    add("Chaotic", "Serviettes",             {2,3,4},          {});
    add("Chaotic", "Live Free or Die",       {2},              {0});
    add("Chaotic", "Long Life",              {3,4,5},          {5});
    add("Chaotic", "Gnarl",                  {1},              {1});

    // Fractal
    add("Fractal", "Replicator",             {1,3,5,7},        {1,3,5,7});
    add("Fractal", "Fredkin",                {1,3,5,7},        {0,2,4,6,8});

    // Growth
    add("Growth",  "Life without Death",     {3},              {0,1,2,3,4,5,6,7,8});
    add("Growth",  "Coral",                  {3},              {4,5,6,7,8});
    add("Growth",  "Land Rush",              {3,5},            {2,3,4,5,7,8});
    add("Growth",  "Bugs",                   {3,5,6,7},        {1,5,6,7,8});
    add("Growth",  "Bacteria",               {3,4},            {4,5,6});

    // Organic
    add("Organic", "Diamoeba",               {3,5,6,7,8},      {5,6,7,8});
    add("Organic", "Amoeba",                 {3,5,7},          {1,3,5,8});
    add("Organic", "Holstein",               {3,5,6,7,8},      {4,6,7,8});
    add("Organic", "Vote",                   {5,6,7,8},        {4,5,6,7,8});

    // Structured
    add("Structured", "Day & Night",         {3,6,7,8},        {3,4,6,7,8});
    add("Structured", "Anneal",              {4,6,7,8},        {3,5,6,7,8});
    add("Structured", "Maze",               {3},              {1,2,3,4,5});
    add("Structured", "Mazectric",           {3},              {1,2,3,4});
    add("Structured", "Gems",               {3,4,5,7},        {4,5,6,8});
    add("Structured", "Stains",              {3,6,7,8},        {2,3,5,6,7,8});
    add("Structured", "Walled Cities",       {4,5,6,7,8},      {2,3,4,5});

    // Other
    add("Other",   "Morley (Move)",          {3,6,8},          {2,4,5});
    add("Other",   "2x2",                   {3,6},            {1,2,5});
    add("Other",   "Coagulations",           {3,7,8},          {2,3,5,6,7,8});
    add("Other",   "Assimilation",           {3,4,5},          {4,5,6,7});

    return presets;
}
