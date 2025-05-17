#include "mineGame.h"
#include "mineDifficulty.h"
#include "mineBoard.h"
#include "mineCell.h"

#include <iostream>
#include <iomanip>
#include <map>
#include <set>

mineBoard mineGame::setupBoard(mineDifficulty::setupValues setupValues)
{
	mineBoard board(
		setupValues.boardHeight,
		setupValues.boardWidth,
		setupValues.mineCount);
	return board;
}

void mineGame::printGameHeader()
{
	int gameWidth{ m_gameBoard.getWidth() * 2 + 2 };
	std::string_view difficultyText{
		mineDifficulty::DifficultyOptions.at(static_cast<int>(m_difficulty)) };
	std::cout << std::setw(gameWidth / 2) << std::left << difficultyText;
	std::cout << std::setw(gameWidth / 2) << std::right << m_theoreticalMinesRemaining;
	std::cout << "\n\n";
}

void mineGame::printGameInstructions()
{
	std::cout << "\n\n";
	std::cout << "Submit moves: <instruction><row><col>\n";
	std::cout << std::setw(25) << std::left << "[(o)pen] tile" << std::endl;
	std::cout << std::setw(25) << std::left << "[(f)lag] mark tile";
	std::cout << playerMarkCharMap.at(playerMark::Flag) << std::endl;
	std::cout << std::setw(25) << std::left << "[(q)uestion] mark tile";
	std::cout << playerMarkCharMap.at(playerMark::Question) << std::endl;
}

void mineGame::getMove()
{
	std::cout << "Submit your move: ";
	std::string userInput;
	std::cin >> userInput;
	std::cout << userInput;
	if (mineGame::validMoveInput(userInput)) {
		//
	}
	else {
		//
	}
}

std::set<std::string> validInstructions{
	"open", "o",
	"flag", "f",
	"question", "q",
};

bool mineGame::validMoveInput(std::string_view userInput)
{
	// valid moves are of the form <instruction><row><col>
	// <instruction> can be open,o,flag,f,question,q
	std::string instruction{};
	std::string row{};
	std::string col{};
	int i{ 0 };

	while (i < userInput.size() and isalpha(userInput[i])) {
		instruction += userInput[i++];
	}
	bool validInstruction{ validateInstruction(instruction) };

	while (i < userInput.size() and isdigit(userInput[i])) {
		row += userInput[i++];
	}
	bool validRow{ validateRow(row) };

	while (i < userInput.size()) {
		col += userInput[i++];
	}
	bool validCol{ validateCol(col) };

	if (!validInstruction || !validRow || !validCol) { return false; }
	return true;
}

static bool isEmpty(std::string_view str) {
	return str.empty();
}

bool mineGame::validateInstruction(const std::string& instruction)
{
	if (isEmpty(instruction)) { return false; }
	if (!validInstructions.contains(instruction)) { return false; }
	return true;
}

bool mineGame::validateRow(const std::string& row)
{
	if (isEmpty(row)) { return false; }
	if (0 > stoi(row) || stoi(row) >= m_gameBoard.getHeight()) { return false; }
	return true;
}

bool mineGame::validateCol(std::string_view col)
{
	if (isEmpty(col)) { return false; }
	if (col.size() > 1) { return false; }

	int asciiColVal{ (int)col[0] };
	if (!isValidColChar(col)) { return false; }
	return true;
}

bool mineGame::isValidColChar(std::string_view input) {
	char c{ input[0] };
	return (isInCharRange(c, 'a', 'a' + m_gameBoard.getWidth())
		|| isInCharRange(c, 'A', 'A' + m_gameBoard.getWidth()));
}

bool mineGame::isInCharRange(const char c, const char min, const char max) {
	return (min < c && c < max);
}

void mineGame::getFirstMove() {
	mineGame::printGameHeader();
	m_gameBoard.displayBoard();
	mineGame::printGameInstructions();
	mineGame::getMove();
}