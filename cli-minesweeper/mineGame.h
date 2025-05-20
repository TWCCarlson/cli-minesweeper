#pragma once
#include "mineDifficulty.h"
#include "mineBoard.h"

#include <iostream>
#include <string>
#include <string_view>

int getValidUserIntInput(int, int, std::string&);

struct playerMove {
	std::string instruction;
	std::string row;
	std::string col;
};

struct parsedPlayerMove {
	char instruction;
	int row;
	int col;
};

const enum class gameStateValues {
	OPENTILE_SUCCESS,
	CONTINUE,
	LOSE,
	WIN
};

class mineGame {
private:
	mineDifficulty::difficultyEnum m_difficulty;
	int m_movesCount{ 0 };
	int m_totalMineCount{};
	int m_theoreticalMinesRemaining;
	int m_uncheckedSafeTileCount{};

	mineBoard setupBoard(mineDifficulty::setupValues setupValues);
	mineBoard m_gameBoard;
	void renderGameState(int row = -1, int col = -1, std::string colorMark = "black");
	void printGameHeader();
	void printGameInstructions();
	parsedPlayerMove getMove();
	playerMove parseUserInput(std::string_view);
	parsedPlayerMove parsePlayerMove(playerMove);
	bool validMoveInput(playerMove);
	bool validateInstruction(const std::string&);
	bool validateRow(const std::string&);
	bool validateCol(std::string_view);
	bool isValidColChar(std::string_view);
	bool isInCharRange(const char, const char, const char);
	gameStateValues executePlayerMove(parsedPlayerMove);
	void checkTrackedValues();
	bool checkIfWon();
	void revealMines();

	std::map<char, gameStateValues(mineGame::*)(int, int)> m_instructionDispatch;
	gameStateValues openTile(int, int);
	gameStateValues flagTile(int, int);
	gameStateValues qmarkTile(int, int);

public:
	mineGame(mineDifficulty::difficultyEnum difficulty) : m_difficulty{ difficulty }
	{
		// operator[] is not allowed on const maps
		// std::map<>.find() returns a std::pair
		// ->first/second accesses the values in the pair
		//auto height_it{ mineDifficulty::HEIGHT.find(difficulty) };
		//auto width_it{ mineDifficulty::WIDTH.find(difficulty) };
		//auto mineCount_it{ mineDifficulty::MINECOUNT.find(difficulty) };

		//// Guard against inappropriate difficulty selection
		//if (height_it == mineDifficulty::HEIGHT.end() ||
		//	width_it == mineDifficulty::WIDTH.end() ||
		//	mineCount_it == mineDifficulty::MINECOUNT.end())
		//{
		//	// Handle this error
		//	std::cout << "ERR: DIFFICULTY NOT FOUND";
		//}
		//mineDifficulty::setupValues s
		//{
		//	height_it->second,
		//	width_it->second,
		//	mineCount_it->second
		//};

		mineDifficulty::setupValues s{};
		int height{};
		int width{};
		int mineCount{};
		if (difficulty == mineDifficulty::difficultyEnum::Custom) {
			std::string heightPrompt{ "Input game board height " };
			std::string widthPrompt{ "Input game board width " };
			std::string mineCountPrompt{ "Input game mine count " };
			height = getValidUserIntInput(2, 26, heightPrompt);
			width = getValidUserIntInput(2, 26, widthPrompt);
			mineCount = getValidUserIntInput(1, height * width - 1, mineCountPrompt);
		}
		else {
			if (!mineDifficulty::HEIGHT.contains(difficulty) ||
				!mineDifficulty::WIDTH.contains(difficulty) ||
				!mineDifficulty::MINECOUNT.contains(difficulty))
			{
				// ERR
			}
			height = mineDifficulty::HEIGHT.at(difficulty);
			width = mineDifficulty::WIDTH.at(difficulty);
			mineCount = mineDifficulty::MINECOUNT.at(difficulty);
		}
		s.boardHeight = height;
		s.boardWidth = width;
		s.mineCount = mineCount;

		m_totalMineCount = mineCount;
		m_theoreticalMinesRemaining = m_totalMineCount;
		m_uncheckedSafeTileCount = (s.boardHeight * s.boardWidth) - s.mineCount;

		// Initialize the move instruction dispatch
		m_instructionDispatch['o'] = &mineGame::openTile;
		m_instructionDispatch['f'] = &mineGame::flagTile;
		m_instructionDispatch['q'] = &mineGame::qmarkTile;

		m_gameBoard = setupBoard(s);
	}
	parsedPlayerMove getFirstMove();
	void placeMines(parsedPlayerMove);
	void calculateTileHints();
	void executeFirstMove(parsedPlayerMove);
	void wonGame();
	void lostGame(parsedPlayerMove);
	void runGameLoop();
};