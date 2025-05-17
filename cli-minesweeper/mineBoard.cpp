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

void mineBoard::printBoardHeader(int colWidth) {
	std::string buffer(3, ' ');
	std::cout << buffer;
	for (int x{ 0 }; x < m_width; x++) {
		std::cout << std::setw(colWidth) << std::left
			<< (char)(x+97);
	}
	std::cout << '\n';
}