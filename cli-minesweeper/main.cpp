#include <iostream>

#include "mineDifficulty.h"
#include "mineGame.h"
#include "mineCell.h"
// CLI Minesweeper

// Game structure
// - Board state needs to be stored and updated
// - - Game object
// - - - Board state (needs a printing method)
// - - - - Cell state
// - - - Data tracking (moves played, mines remaining)
// - Board state needs to be displayed
// - User input comes from cin
// - - Needs to be parsed, could use chess-style notation

void introScreen() {
	bool gameStart{ false };
	while (!gameStart) {
		std::cout << "Welcome to CLI minesweeper.\n\n";
		std::cout << "The game will be played using your keyboard.\n";
		std::cout << "Input the instruction you choose (open / flag)"
			" along with the code for the tile. \nOpen all tiles"
			" not containing mines to win!\n\n";
		std::cout << "Hit 'r' to begin: ";
		std::string readyCheck{};
		std::cin >> readyCheck;
		gameStart = (readyCheck == "r");
		system("cls");
	}
}

mineDifficulty::difficultyEnum selectDifficulty() {
	while (true) {
		int difficultyValue{ 0 };
		std::cout << "Choose your difficulty level:\n\n";
		for (auto [key, val] : mineDifficulty::DifficultyOptions) {
			std::cout << key << ": " << val << '\n';
		}

		std::cout << "\nEnter the corresponding number: ";
		std::cin >> difficultyValue;

		system("cls");
		// Check the entered value exists in the map keys
		if (mineDifficulty::DifficultyOptions.contains(difficultyValue)) {
			return static_cast<mineDifficulty::difficultyEnum>(difficultyValue);
		}
	}
}

int main() {
	// Prompt user to set up the game
	introScreen();
	mineDifficulty::difficultyEnum difficultyLevel{ selectDifficulty() };

	// Create the game instance
	mineGame game{ difficultyLevel };
	// Create the board
	// Create the cells
	// Await first move
	parsedPlayerMove firstMove{ game.getFirstMove() };
	// Disperse mines (avoid first revealed cell)
	game.placeMines(firstMove);
	game.calculateTileHints();
	// Loop until game end
	//game.runGameLoop();
	// - All non-mine cells revealed
	// - Mine revealed
	return 0;
}