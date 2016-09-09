#pragma once

#include "Common.h"

class Board;
typedef std::unique_ptr<Board> upBoard;

#include "Game.h"
#include "Square.h"
#include "Piece.h"

class Board : private Uncopyable
{
	// Square data
	struct Square
	{
		SquarePosition position;
		spSprite sprite;
	};

public:
	// Initialization
	Board();
	virtual ~Board();

public:
	// Pieces and squares
	void reset();
	shpPiece piece(NumericId id) const;
	size_t piecesNumber() const;
	NumericId kingId(PlayerSide side) const;
	NumericId pieceOnSquare(SquarePosition square) const;
	
public:
	// Path testing
	bool diagonalPathIsFree(SquarePosition start, SquarePosition end) const;
	bool horizontalPathIsFree(SquarePosition start, SquarePosition end) const;
	bool verticalPathIsFree(SquarePosition start, SquarePosition end) const;
	
public:
	// Square highlighting
	void highlightPieceMoves(NumericId pieceId);
	void highlightPieces(PlayerSide side);

public:
	// Selection
	void selectPiece(NumericId id);
	NumericId selectedPieceId() const;

public:
	// Actors
	spActor base();
	spActor square(SquarePosition position);
	void updateTimeIndicator(PlayerSide side, timeMS time);

protected:
	// Implementation
	void respawnPieces();
	void spawnSidePieces(PlayerSide side);
	void spawnPiece(Piece* piece, PlayerSide side, SquarePosition position);

protected:
	// Implementation (event handling)
	void onSquareClick(Event* e);

private:
	// Fields
	Square m_squares[BOARD_SIZE * BOARD_SIZE];
	spSprite m_base;
	std::vector<shpPiece> m_pieces;
	NumericId m_selectedPieceId;
	NumericId m_whiteKingId;
	NumericId m_blackKingId;
	NumericId m_nextPieceId;
	spTextField m_timeIndicator;
	Vector2 m_whiteIndicatorPosition;
	Vector2 m_blackIndicatorPosition;
};
