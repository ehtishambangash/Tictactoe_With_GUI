# Tic Tac Toe Game

This project is a graphical Tic Tac Toe game implemented in C++ using the **SFML** library. The game allows two players to compete, keeps track of their scores, and saves game statistics to a file.

## Features

- **Two-Player Mode**: Players can enter their names and take turns playing.
- **Graphical Interface**: The game uses images, sprites, and text to display the game board and status.
- **Persistent Stats**: Game statistics (total games, wins, and draws) are saved to a file and loaded at the start of the game.
- **Reset Functionality**: Players can reset the board after a game is completed.

## Getting Started

### Prerequisites

- **C++ Compiler**: Ensure you have a C++ compiler installed that supports C++17 or later.
- **SFML Library**: Download and install the SFML library from [https://www.sfml-dev.org/](https://www.sfml-dev.org/).
- **Windows Environment**: The program uses Windows-specific functions, so it may not work directly on other operating systems without modifications.

### Files Required

Ensure the following files are in the `Resources` directory:

1. `background.png`: The background image for the game.
2. `board.png`: The Tic Tac Toe board image.
3. `reset.png`: The reset button image.
4. `circle.png`: The image representing Player 2's marker.
5. `cross.png`: The image representing Player 1's marker.
6. `blank.png`: The image representing empty board slots.
7. `font.ttf`: The font file used for displaying text.

Additionally, the game writes and reads statistics from a `game_stats.txt` file in the same directory as the executable.

### Building the Project

1. Clone the repository or copy the source code.
2. Place all required resources in the `Resources` directory.
3. Compile the code using a compiler that supports SFML. Below is an example command for compilation:

   ```bash
   g++ -o TicTacToe main.cpp -lsfml-graphics -lsfml-window -lsfml-system
   ```

4. Run the compiled executable:

   ```bash
   ./TicTacToe
   ```

### Controls

- **Mouse Left Click**:
  - Click on an empty slot to place your marker.
  - Click the reset button to reset the board after a game.

- **Close Window**: Close the game by clicking the window's close button.

## Code Overview

### Key Components

1. **`game_assets` Structure**:
   - Stores game assets like images, textures, and fonts.

2. **`game_sprites` Structure**:
   - Stores the sprites and text elements used in the game.

3. **`game_state` Structure**:
   - Tracks the game state, including the current player, board status, and game statistics.

4. **Main Functions**:
   - `load_assets`: Loads all required assets.
   - `load_board`: Initializes or resets the game board.
   - `check_win`: Checks if the current player has won.
   - `check_draw`: Checks if the game is a draw.
   - `key_press`: Handles user interactions.
   - `load_game_stats` and `save_game_stats`: Handle reading and writing of game statistics.

### Main Function

- Takes player names as input.
- Loads game assets and statistics.
- Initializes the game board and launches a graphical window using SFML.

### Game Flow

1. Players alternate placing their markers (cross or circle) on the board.
2. The game checks for wins or draws after each move.
3. Once a game ends, the reset button becomes active to restart the board.

## Game Statistics

Game statistics are saved in the `game_stats.txt` file and include:

- Total games played.
- Games won by Player 1.
- Games won by Player 2.
- Number of draw games.

## Future Improvements

- Add a main menu for better navigation.
- Allow custom player markers.
- Support AI for single-player mode.
- Make the game cross-platform by removing Windows-specific dependencies.

## Troubleshooting

1. **Assets Not Found**:
   Ensure the `Resources` directory contains all required files.
2. **SFML Errors**:
   Verify the SFML library is installed correctly and linked during compilation.
3. **Statistics Not Saved**:
   Check file permissions for `game_stats.txt`.

## License

This project is free to use and modify for personal or educational purposes.

## Acknowledgments

- [SFML Library](https://www.sfml-dev.org/): For providing the graphical framework used in this game.
- Developers of various open-source projects for inspiration.
