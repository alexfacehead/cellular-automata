# Cellular Automata Playground 🎉

Welcome to the shitty Cellular Automata project! This is a fun and simple project inspired by Stephen Wolfram's research on cellular automata. Dive into the world of simple rules and complex patterns, and explore the fascinating emergent behaviors that arise from these tiny cells.

## What is Cellular Automata? 🤔

Cellular automata (CA) is a discrete model studied in computer science, mathematics, and theoretical biology. It consists of a grid of cells, each in one of a finite number of states, such as on and off. The grid can be any finite number of dimensions. For each cell, a set of cells called its neighborhood is defined relative to the specified cell. An initial state (time t = 0) is selected by assigning a state for each cell. A new generation is created (advancing t by 1), according to some fixed rule that determines the new state of each cell in terms of the current state of the cell and the states of the cells in its neighborhood.

## Getting Started 🚀

This project provides a C++ implementation of 2D cellular automata with customizable rules and initial states. You can explore different rulesets, such as Conway's Game of Life or create your own unique rulesets to see the fascinating patterns that emerge.

To get started, simply compile and run the provided code. You can customize the grid size, initial states, and rulesets as you see fit. Have fun experimenting with different patterns and rules, and watch as your grid comes to life!

Run:
`g++ -std=c++11 -o cellular_automata Main.cpp Grid.cpp RuleSet.cpp Cell.cpp -lsfml-graphics -lsfml-window -lsfml-system`

Then run:
`./cellular_automata` on Linux/WSL
OR
`.\cellular_automata` on Windows (if I recall correctly, apologies, I strongly recommend using https://learn.microsoft.com/en-us/windows/wsl/install)

## Acknowledgments 🙌

This project is inspired by the work of Stephen Wolfram, a pioneer in the field of cellular automata research. His groundbreaking discoveries have shed light on the complex behaviors that can arise from simple rules, and have inspired countless researchers and enthusiasts alike.

One of my personal goals is to understand C++ well enough to be able to efficiently render his recent research regarding 3D hypergraphs. These things are amazing: computatinal irreducibility, incredible emergent phenomena, and potentially a new framework for physics are just the tip of the iceberg. `directed-graph-3D` is a very, very simple example of computations that end up incredibly complex with simple rules to start.

## TO DO

- Make it easier to alter the simulation (ETA 24-72 hours)
- Add colors!
- Add new RuleSets and a cooler UI

## License 📄

This project is licensed under the MIT License. Feel free to use, modify, and distribute the code as you see fit. Happy coding! 😄

-----------

This README was optimized by GPT-4.