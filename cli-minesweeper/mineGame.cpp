#include "mineGame.h"
#include "mineDifficulty.h"
#include "mineBoard.h"
#include "mineCell.h"

#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <random>
#include <algorithm>

mineBoard mineGame::setupBoard(mineDifficulty::setupValues setupValues)
{
	mineBoard board(
		setupValues.boardHeight,
		setupValues.boardWidth,
		setupValues.mineCount);
	return board;
}

void mineGame::renderGameState()
{
	system("cls");
	printGameHeader();
	m_gameBoard.displayBoard();
	printGameInstructions();
}

void mineGame::printGameHeader()
{
	int gameWidth{ m_gameBoard.getWidth() * 2 + 2 };
	std::string_view difficultyText{
		mineDifficulty::DifficultyOptions.at(static_cast<int>(m_difficulty)) };
	std::cout << std::setw(gameWidth / 2) << std::left << difficultyText;
	std::cout << std::setw(gameWidth / 2) << std::right << "Mines left: " << m_theoreticalMinesRemaining;
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

parsedPlayerMove mineGame::getMove()
{
	bool gotValidMove{ false };
	playerMove playerMove{};
	do {
		std::cout << "Submit your move: ";
		std::string userInput;
		std::cin >> userInput;
		playerMove = parseUserInput(userInput);
		gotValidMove = validMoveInput(playerMove);
	} while (!gotValidMove);
	parsedPlayerMove pplayerMove{ parsePlayerMove(playerMove) };
	return pplayerMove;
}

std::set<std::string> validInstructions{
	"open", "o",
	"flag", "f",
	"question", "q",
};

playerMove mineGame::parseUserInput(std::string_view userInput) {
	std::string instruction{};
	std::string row{};
	std::string col{};
	int i{ 0 };

	// Extract instruction: only alpha chars
	while (i < userInput.size() and isalpha(userInput[i])) {
		// ERR
		instruction += userInput[i++];
	}

	// Extract row identifier: only digits
	while (i < userInput.size() and isdigit(userInput[i])) {
		// ERR
		row += userInput[i++];
	}

	// Extract col identifier: only a single alpha char
	while (i < userInput.size()) {
		// ERR
		col += userInput[i++];
	}

	playerMove playerMove{
		.instruction = instruction,
		.row = row,
		.col = col };
	return playerMove;
}

parsedPlayerMove mineGame::parsePlayerMove(playerMove playerMove)
{
	parsedPlayerMove pPlayerMove{
		.instruction = playerMove.instruction[0], // first char
		.row = stoi(playerMove.row), // string->int[0-rows]
		.col = (int)playerMove.col[0] - 'a' // char->int normalized[0-cols]
	};
	return pPlayerMove;
}

bool mineGame::validMoveInput(playerMove playerMove)
{
	// valid moves are of the form <instruction><row><col>
	// <instruction> can be open,o,flag,f,question,q
	bool validInstruction{ validateInstruction(playerMove.instruction) };
	bool validRow{ validateRow(playerMove.row) };
	bool validCol{ validateCol(playerMove.col) };

	return (validInstruction && validRow && validCol);
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
	return (min <= c && c <= max);
}

gameStateValues mineGame::executePlayerMove(parsedPlayerMove ppMove)
{
	// Use the dispatch defined in constructor to call the action function
	gameStateValues result{ gameStateValues::CONTINUE };
	result = (this->*m_instructionDispatch[ppMove.instruction])(ppMove.row, ppMove.col);
	return result;
}

gameStateValues mineGame::openTile(int row, int col)
{
	mineCell& targetTile = m_gameBoard.getTile(row, col);
	if (targetTile.getPlayerVisibility()) { return gameStateValues::CONTINUE; }
	targetTile.openTile();
	if (targetTile.getMineState()) { return gameStateValues::LOSE; }
	// If a tile has no mine and no hint, it has no neighboring mines
	// In minesweeper this means all adjacent tiles should be opened
	// Essentially BFS/DFS where the stop condition is a hint
	if (targetTile.getNumberOfNeighborMines() == 0) { m_gameBoard.openTileChain(row, col); }
	return gameStateValues::OPENTILE_SUCCESS;
}

gameStateValues mineGame::flagTile(int row, int col)
{
	m_gameBoard.getTile(row, col).flagTile();
	return gameStateValues::CONTINUE;
}

gameStateValues mineGame::qmarkTile(int row, int col)
{
	m_gameBoard.getTile(row, col).qmarkTile();
	return gameStateValues::CONTINUE;
}

parsedPlayerMove mineGame::getFirstMove()
{
	// Must open one cell before mine placement
	parsedPlayerMove ppMove{};
	do {
		renderGameState();
		ppMove = getMove();
		m_uncheckedSafeTileCount--;
	} while (ppMove.instruction != 'o');
	return ppMove;
}

void mineGame::placeMines(parsedPlayerMove ppMove)
{
	// The board stores tiles in a vector
	// It is sufficient to randomize the order of a vector of pointers
	// Then select the first N pointers in the shuffled vector for mines
	std::vector<mineCell*> shuffledTiles{ m_gameBoard.getCellPointers() };

	// Set URBG (Uniform Random Number Generator)
	std::random_device rd;
	std::mt19937 urbg(rd());

	// Get pointer to first move tile
	mineCell* bannedTile{ &m_gameBoard.getTile(ppMove.row, ppMove.col) };

	std::shuffle(shuffledTiles.begin(), shuffledTiles.end(), urbg);
	int placedCount{ 0 };
	for (int count = 0; placedCount < m_gameBoard.getMineCount(); count++) {
		mineCell* targetTile = shuffledTiles[count];
		// Mines should never appear under first tile opened
		if (targetTile == bannedTile) {}
		else { (*targetTile).placeMine(); placedCount++; }
	}
}

void mineGame::calculateTileHints()
{
	// Determine the 'hint' number for each tile
	// The number is the count of mines in 8-neighbor adjacency
	// Need to guard against exceeding vector bounds
	// Need to guard against wrapping as the board is a vector
	// but column 1 has no left-adjacent tiles
	std::vector<mineCell>& tileList{ m_gameBoard.getBoard() };
	int neighborCount{ 0 };
	std::vector<int> neighborsToCheck{};
	for (int idx = 0; idx < tileList.size(); idx++) {
		neighborCount = 0;
		neighborsToCheck = m_gameBoard.getValidChebyshevNeighborIDX(idx);
		for (int neighborIDX : neighborsToCheck) {
			if (tileList[neighborIDX].getMineState()) { neighborCount++; }
		}
		tileList[idx].setNumberOfNeighborMines(neighborCount);
	}
}

void mineGame::executeFirstMove(parsedPlayerMove ppMove)
{
	executePlayerMove(ppMove);
}

void mineGame::runGameLoop()
{
	gameStateValues gameStatusCode{ gameStateValues::CONTINUE };
	while (true) {
		renderGameState();
		gameStatusCode = executePlayerMove(getMove());

		if (gameStatusCode == gameStateValues::OPENTILE_SUCCESS) {
			m_uncheckedSafeTileCount--;
			if (m_uncheckedSafeTileCount == 0) {
				gameStatusCode = gameStateValues::WIN;
			}
		}

		if (gameStatusCode == gameStateValues::LOSE) {
			std::cout << "\nYou stepped on a mine!\n\n";
			break;
		}

		if (gameStatusCode == gameStateValues::WIN) {
			std::cout << "\nYou won!\n\n";
			break;
		}
	}
}
