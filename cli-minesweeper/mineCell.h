#pragma once

#include <map>

const enum class playerMark {
	None,
	Flag,
	Question
};

extern const std::map<playerMark, char> playerMarkCharMap;

class mineCell {
private:
	bool m_hasMine{ false };
	bool m_isRevealed{ false };
	int m_numberOfNeighborMines{ 0 };
	playerMark m_playerMark{ playerMark::None };
	char getPlayerMarkChar();
	char getGameMarkChar();

public:
	mineCell() : m_hasMine{} {};
	void printCell(int colWidth);
};
