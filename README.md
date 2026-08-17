# LCR Dice Game

LCR Dice Game is a console-based implementation of the Left Center Right dice game written in C++.

Players enter their names, take turns rolling dice, and pass chips left, right, or into the center pot based on each roll. The enhanced version expands the original single-game program with tournament play, player statistics, leaderboard ranking, stronger input validation, and more modular game logic.

## Technologies

- C++
- Visual Studio
- Standard Template Library (STL)

## Features

### Core Gameplay

- Supports three or more players
- Collects player names before gameplay begins
- Simulates Left, Center, Right, and dot dice results
- Tracks chips for each player
- Tracks chips placed into the center pot
- Continues gameplay until only one player has chips remaining
- Displays game rules from an external text file

### Tournament Mode

- Allows multiple games to be played during one program session
- Tracks the number of games played by each player
- Tracks the number of wins for each player
- Tracks the total number of winning chips earned
- Updates tournament statistics after each completed game

### Leaderboard

- Builds a final leaderboard from tournament statistics
- Sorts players by total wins
- Uses chips won as a secondary ranking condition
- Uses player name as a final alphabetical tie-breaker
- Displays tournament results after the player chooses to stop playing

### Input Validation

- Requires at least three players
- Rejects invalid nonnumeric player-count input
- Rejects incomplete or malformed numeric input
- Prevents gameplay from beginning until a valid player count is entered

## Program Design

The project uses classes, structures, standard library containers, and reusable functions to organize game behavior.

### Player and Dice Objects

The game uses classes to represent player information and dice behavior. This separates responsibilities and makes the gameplay logic easier to understand and maintain.

### Tournament Statistics

Player statistics are stored using an `unordered_map`, allowing each player's name to be associated with tournament information such as:

- Games played
- Wins
- Chips won

### Game Results

Each completed game returns a structured result containing information about the winner and the final game state. This allows gameplay logic and tournament-statistics logic to remain separate.

### Leaderboard Sorting

Tournament statistics are converted into a vector and sorted with `std::sort`.

The ranking logic uses:

1. Wins in descending order
2. Chips won in descending order
3. Player name in alphabetical order

This demonstrates custom comparison logic and practical use of standard C++ algorithms.

## C++ Skills Demonstrated

This project demonstrates experience with:

- Object-oriented programming
- Classes and objects
- Encapsulation
- Structures
- `std::vector`
- `std::unordered_map`
- `std::sort`
- Lambda expressions
- File input
- Random number generation
- Input validation
- String parsing
- Loops and conditional logic
- Function decomposition
- Reusable game logic
- Custom sorting algorithms
- Basic statistics tracking

## Running the Project

### Requirements

- Visual Studio with C++ development tools installed

### Open the Project

Open the solution file in Visual Studio:

    LCRFinalProject.slnx

Build the solution and run the project from Visual Studio.

The program also uses:

    LCRRules.txt

Keep this file in the project directory so the game can display its rules when the program starts.

## Project Structure

    lcr-dice-game/
    ├── LCRFinalProject/
    │   ├── LCRFinalProject.cpp
    │   ├── LCRFinalProject.vcxproj
    │   ├── LCRFinalProject.vcxproj.filters
    │   └── LCRRules.txt
    ├── .gitignore
    ├── LCRFinalProject.slnx
    └── README.md

## Project Evolution

The original version of the application focused on implementing the rules and flow of a single Left Center Right game.

The enhanced version expands the application into a tournament system. Gameplay was separated into reusable functions, game results were structured so they could be processed independently, and player statistics were stored across multiple rounds.

The enhancement also introduced a ranked leaderboard using C++ standard library containers and sorting algorithms. More robust input validation was added to prevent invalid player counts and malformed input from interrupting gameplay.

These changes improved the structure, maintainability, and functionality of the original program while demonstrating practical use of algorithms and data structures in C++.
