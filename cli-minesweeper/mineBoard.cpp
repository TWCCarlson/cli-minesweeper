#include "mineBoard.h"
#include <iostream>
#include <iomanip>

void mineBoard::displayBoard() {
	mineBoard::printBoardHeader(2);
	for (int y{ 0 }; y < m_height; y++) {
		std::cout << std::setw(2) << std::right << y << " ";
		for (int x{ 0 }; x < m_width; x++) {
			m_board[y * m_width + x].printCell(2);
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

std::vector<int> mineBoard::getValidNeighborIDX(int idx)
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

bool mineBoard::isValidNeighbor(int row, int col)
{
	if (row < 0) { return false; }
	if (row >= m_height) { return false; }
	if (col < 0) { return false; }
	if (col >= m_width) { return false; }
	return true;
}

void mineBoard::printBoardHeader(int colWidth) {
	std::string buffer(3, ' ');
	std::cout << buffer;
	for (int x{ 0 }; x < m_width; x++) {
		std::cout << std::setw(colWidth) << std::left
			<< (char)(x + 97);
	}
	std::cout << '\n';
}