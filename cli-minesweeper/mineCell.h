#pragma once

#include <map>
#include <iostream>

const enum class playerMark {
	None,
	Flag,
	Question
};

extern const std::map<playerMark, char> playerMarkCharMap;

const std::map<char, std::string> hintColorMap{
	{'1', "light blue"},
	{'2', "green"},
	{'3', "red"},
	{'4', "purple"},
	{'5', "light red"},
	{'6', "aqua"},
	{'7', "light purple"},
	{'8', "yellow"}
};

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
	void openTile() { m_isRevealed = true; m_playerMark = playerMark::None; };
	void flagTile() { m_playerMark = playerMark::Flag; }
	void qmarkTile() { m_playerMark = playerMark::Question; }
	void placeMine() { m_hasMine = true; m_isRevealed = true; };
	const bool getMineState() const { return m_hasMine; };
	void setNumberOfNeighborMines(int c) { m_numberOfNeighborMines = c; }
	const int getNumberOfNeighborMines() const { return m_numberOfNeighborMines; }
	bool getPlayerVisibility() { return m_isRevealed; };
	playerMark getPlayerMark() { return m_playerMark; }
	bool isEmpty() { return (!m_hasMine && m_numberOfNeighborMines == 0); }
};
