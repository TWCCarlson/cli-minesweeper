#pragma once
#include "mineDifficulty.h"
#include "mineBoard.h"

#include <iostream>
#include <string>
#include <string_view>

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

class mineGame {
private:
	mineDifficulty::difficultyEnum m_difficulty;
	int m_movesCount{ 0 };
	int m_totalMineCount{};
	int m_theoreticalMinesRemaining;

	mineBoard setupBoard(mineDifficulty::setupValues setupValues);
	mineBoard m_gameBoard;
	void renderGameState();
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
	void executePlayerMove(parsedPlayerMove);

	std::map<char, void(mineGame::*)(int, int)> m_instructionDispatch;
	void openTile(int, int);
	void flagTile(int, int);
	void qmarkTile(int, int);

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

		if (!mineDifficulty::HEIGHT.contains(difficulty) ||
			!mineDifficulty::WIDTH.contains(difficulty) ||
			!mineDifficulty::MINECOUNT.contains(difficulty))
		{
			// ERR
		}

		m_totalMineCount = mineDifficulty::MINECOUNT.at(difficulty);
		m_theoreticalMinesRemaining = m_totalMineCount;
		mineDifficulty::setupValues s{
			mineDifficulty::HEIGHT.at(difficulty),
			mineDifficulty::WIDTH.at(difficulty),
			m_totalMineCount
		};

		// Initialize the move instruction dispatch
		m_instructionDispatch['o'] = &mineGame::openTile;
		m_instructionDispatch['f'] = &mineGame::flagTile;
		m_instructionDispatch['q'] = &mineGame::qmarkTile;

		m_gameBoard = setupBoard(s);
	}
	parsedPlayerMove getFirstMove();
	void placeMines(parsedPlayerMove);
	void calculateTileHints();
	void runGameLoop();
};