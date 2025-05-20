#include "mineBoard.h"
#include <iostream>
#include <iomanip>
#include <functional>

void mineBoard::printBoardHeader(int colWidth) {
	std::string buffer(3, ' ');
	std::cout << buffer;
	for (int x{ 0 }; x < m_width; x++) {
		std::cout << std::setw(colWidth) << std::left
			<< (char)(x + 97);
	}
	std::cout << '\n';
}

void mineBoard::displayBoard(int row, int col, std::string color) {
	mineBoard::printBoardHeader(2);
	for (int r{ 0 }; r < m_height; r++) {
		std::cout << std::setw(2) << std::right << r << " ";
		for (int c{ 0 }; c < m_width; c++) {
			if (r == row && c == col) {
				m_board[r * m_width + c].printCell(2, color);
			}
			else {
				m_board[r * m_width + c].printCell(2);
			}
		}
		std::cout << '\n';
	}
}

std::vector<mineCell*> mineBoard::getCellPointers()
{
	std::vector<mineCell*> pointers;
	pointers.reserve(m_board.size()); // Preallocate
	for (mineCell& cell : m_board) { // Reference avoids copies
		pointers.push_back(&cell);
	}
	return pointers;
}

mineCell& mineBoard::getTile(int row, int col)
{
	int idx{ row * m_width + col };
	return m_board[idx];
}

std::vector<int> mineBoard::getValidChebyshevNeighborIDX(int idx)
{
	std::vector<int> validNeighbors{};
	int row{ idx / m_width };
	int col{ idx % m_width };
	for (int r{ row - 1 }; r <= row + 1; r++) {
		for (int c{ col - 1 }; c <= col + 1; c++) {
			if (r == row && c == col) { continue; } // Ignore self
			if (isValidNeighbor(r, c)) {
				validNeighbors.push_back(r * m_width + c);
			}
		}
	}
	return validNeighbors;
}

std::vector<int> mineBoard::getValidOrthogonalNeighborIDX(int idx)
{
	std::vector<int> validNeighbors{};
	int row{ idx / m_width };
	int col{ idx % m_width };
	for (int r{ row - 1 }; r <= row + 1; r++) {
		if (r == row) { continue; } // Ignore self
		if (isValidNeighbor(r, col)) {
			validNeighbors.push_back(r * m_width + col);
		}
	}
	for (int c{ col - 1 }; c <= col + 1; c++) {
		if (c == col) { continue; } // Ignore self
		if (isValidNeighbor(row, c)) {
			validNeighbors.push_back(row * m_width + c);
		}
	}
	return validNeighbors;
}

bool mineBoard::isValidNeighbor(int row, int col)
{
	if (row < 0) { return false; }
	if (row >= m_height) { return false; }
	if (col < 0) { return false; }
	if (col >= m_width) { return false; }
	return true;
}

void mineBoard::openTileChain(int row, int col)
{
	// DFS
	// Store which tiles have been checked already
	// && .isRevealed()?
	std::map<int, bool> isChecked{};

	// Define recursive function for DFS
	std::function<void(int, int)> dfs;
	dfs = [this, &isChecked, &dfs](int row, int col)
		{
			int idx{ row * this->m_width + col };
			isChecked[idx] = true;
			std::vector<int> adjTiles{ this->getValidChebyshevNeighborIDX(idx) };
			for (int adjIDX : adjTiles) {
				mineCell& adjTile = this->m_board[adjIDX];

				// When encountering a new cell
				// - if cell has been explored, ignore it
				// - if there is a mine, don't reveal
				// - if there is a hint, reveal it
				// - if there is no hint, reveal and search more
				if (isChecked[adjIDX] || adjTile.getMineState()) { continue; }
				adjTile.openTile();
				if (adjTile.isEmpty()) { dfs((adjIDX / m_width), (adjIDX % m_width)); }
			}
		};
	dfs(row, col);
}