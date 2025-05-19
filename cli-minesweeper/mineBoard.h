#pragma once
#include "mineCell.h"

#include <iostream>
#include <vector>

class mineBoard {
private:
	int m_height;
	int m_width;
	int m_numberOfMines;
	std::vector<mineCell> m_board;
	void printBoardHeader(int colWidth);

public:
	mineBoard(int height = 9, int width = 9, int mineCount = 10) :
		m_height{ height },
		m_width{ width },
		m_numberOfMines{ mineCount },
		m_board(height* width, mineCell()) {
	};
	void displayBoard();
	std::vector<mineCell>& getBoard() { return m_board; };
	std::vector<mineCell*> getCellPointers();
	const int getWidth() { return m_width; };
	const int getHeight() { return m_height; };
	mineCell& getTile(int, int);
	const int getMineCount() { return m_numberOfMines; };
	std::vector<int> getValidChebyshevNeighborIDX(int);
	std::vector<int> getValidOrthogonalNeighborIDX(int);
	bool isValidNeighbor(int, int);
	void openTileChain(int, int);
};