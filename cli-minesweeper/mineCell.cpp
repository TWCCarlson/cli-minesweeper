#include "mineCell.h"
#include "color.hpp"

#include <iostream>
#include <iomanip>

void mineCell::printCell(int colWidth) {
	std::cout << std::setw(colWidth) << std::left;

	if (!m_isRevealed)
	{
		std::cout << getPlayerMarkChar();
	}
	else {
		char gameMarkChar{ getGameMarkChar() };
		if (hintColorMap.contains(gameMarkChar)) {
			std::string color{ hintColorMap.at(gameMarkChar) };
			std::cout << dye::colorize(gameMarkChar, color);
		}
		else {
			std::cout << (gameMarkChar);
		}
	}
}

char mineCell::getPlayerMarkChar() {
	return playerMarkCharMap.at(m_playerMark);
}

char mineCell::getGameMarkChar() {
	if (m_hasMine) { return (char)232; };
	// If there are 0 neighbor mines the char is blank
	if (m_numberOfNeighborMines == 0) { return ' '; }
	// Otherwise cast from int to char
	return '0' + (char)m_numberOfNeighborMines;
}

const std::map<playerMark, char> playerMarkCharMap{
	{playerMark::None, (char)178},
	{playerMark::Flag, (char)251},
	{playerMark::Question, (char)63}
};