#include "mineCell.h"

#include <iostream>
#include <iomanip>

void mineCell::printCell(int colWidth) {
	std::cout << std::setw(colWidth) << std::left;

	if (!m_isRevealed)
	{
		std::cout << getPlayerMarkChar();
	}
	else {
		std::cout << getGameMarkChar();
	}
}

char mineCell::getPlayerMarkChar() {
	return playerMarkCharMap.at(m_playerMark);
}

char mineCell::getGameMarkChar() {
	// If there are 0 neighbor mines the char is blank
	if (m_numberOfNeighborMines == 0) {
		return ' ';	
	}
	// Otherwise cast from into char
	return '0' + (char)m_numberOfNeighborMines;
}

const std::map<playerMark, char> playerMarkCharMap{
	{playerMark::None, (char)178},
	{playerMark::Flag, (char)251},
	{playerMark::Question, (char)63}
};