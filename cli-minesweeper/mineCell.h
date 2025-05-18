#pragma once

#include <map>
#include <iostream>

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
	void openTile() { m_isRevealed = true; };
	void flagTile() { m_playerMark = playerMark::Flag; }
	void qmarkTile() { m_playerMark = playerMark::Question; }
	void placeMine() { m_hasMine = true; m_isRevealed = true; };
};
