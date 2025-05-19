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
	int difficultyValue{};
	while (true) {
		std::cout << "Choose your difficulty level:\n\n";
		for (auto [key, val] : mineDifficulty::DifficultyOptions) {
			std::cout << key << ": " << val << '\n';
		}

		std::cout << "\nEnter the corresponding number: ";
		std::cin >> difficultyValue;
		if (!std::cin.fail() && mineDifficulty::DifficultyOptions.contains(difficultyValue))
		{
			break;
		}
		std::cin.clear(); // Clear fail flag
		std::cin.ignore();// Clear buffer
		system("cls");
	}
	return static_cast<mineDifficulty::difficultyEnum>(difficultyValue);
}


int main() {
	// Prompt user to set up the game
	introScreen();
	mineDifficulty::difficultyEnum difficultyLevel{ selectDifficulty() };

	// Create the game instance
	mineGame game{ difficultyLevel };

	// Await first move
	parsedPlayerMove firstMove{ game.getFirstMove() };

	// Disperse mines (avoid first revealed cell)
	game.placeMines(firstMove);
	game.calculateTileHints();

	// Calculate effects of the first move
	game.executeFirstMove(firstMove);

	// Loop until game end
	game.runGameLoop();
	return 0;
}