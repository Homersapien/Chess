#pragma once

#include "Common.h"

class Match;
typedef std::unique_ptr<Match> upMatch;

#include "Game.h"

class Match : private Uncopyable
{
public:
	// Initialization
	Match();
	virtual ~Match();

public:
	// State
	void reset();
	GameState state() const;
	void update();
	void finish(const std::string& reason);
	PlayerSide currentPlayer() const;

public:
	// Making a move
	timeMS moveStartTime() const;
	void makeMove(NumericId pieceId, SquarePosition targetPosition);

protected:
	// Implementation (win conditions)
	void checkWinConditions();
	bool playerCanMakeMove(PlayerSide side) const;
	bool kingIsInCheck(PlayerSide side) const;

private:
	// Fields
	GameState m_state;
	PlayerSide m_currentPlayer;
	timeMS m_moveStartTime;
	timeMS m_indicatorNextUpdateTime;
};

