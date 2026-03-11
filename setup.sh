#!/usr/bin/env bash
set -e

echo "=== Cellular Automata — Setup & Build ==="
echo

OS="$(uname -s)"

# --- Install dependencies ---
if [ "$OS" = "Darwin" ]; then
    echo "[1/3] Checking for Homebrew..."
    if ! command -v brew &>/dev/null; then
        echo "Homebrew not found. Installing..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    echo "[2/3] Installing SFML 2.x via Homebrew..."
    if brew list sfml@2 &>/dev/null; then
        echo "  sfml@2 already installed."
    else
        brew install sfml@2
    fi

    echo "[3/3] Compiling..."
    g++ -std=c++17 -O2 -o cellular_automata \
        Main.cpp Grid.cpp RuleSet.cpp Cell.cpp Menu.cpp Settings.cpp \
        -I/opt/homebrew/opt/sfml@2/include \
        -L/opt/homebrew/opt/sfml@2/lib \
        -lsfml-graphics -lsfml-window -lsfml-system

elif [ "$OS" = "Linux" ]; then
    echo "[1/3] Detecting package manager..."

    if command -v apt &>/dev/null; then
        echo "[2/3] Installing SFML via apt..."
        sudo apt update -qq
        sudo apt install -y g++ libsfml-dev
    elif command -v dnf &>/dev/null; then
        echo "[2/3] Installing SFML via dnf..."
        sudo dnf install -y gcc-c++ SFML-devel
    elif command -v pacman &>/dev/null; then
        echo "[2/3] Installing SFML via pacman..."
        sudo pacman -S --noconfirm gcc sfml
    else
        echo "ERROR: Unsupported Linux package manager."
        echo "Please install g++ and SFML 2.x manually, then run:"
        echo "  g++ -std=c++17 -O2 -o cellular_automata Main.cpp Grid.cpp RuleSet.cpp Cell.cpp Menu.cpp Settings.cpp -lsfml-graphics -lsfml-window -lsfml-system"
        exit 1
    fi

    echo "[3/3] Compiling..."
    g++ -std=c++17 -O2 -o cellular_automata \
        Main.cpp Grid.cpp RuleSet.cpp Cell.cpp Menu.cpp Settings.cpp \
        -lsfml-graphics -lsfml-window -lsfml-system
else
    echo "ERROR: Unsupported OS '$OS'. Use setup.bat on Windows."
    exit 1
fi

echo
echo "Build successful! Run with:"
echo "  ./cellular_automata"
