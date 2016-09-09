#pragma once

#include "Common.h"

class Piece;
typedef std::shared_ptr<Piece> shpPiece;

#include "Game.h"
#include "Board.h"

enum PieceState
{
	PIECE_STATE_NORMAL,
	PIECE_STATE_CAPTURED
};

class Piece : private Uncopyable
{
public:
	// Construction
	Piece();
	virtual ~Piece();
	virtual void init(NumericId id, PlayerSide side, SquarePosition square);

public:
	// Parameters
	NumericId id() const;
	Board* board() const;
	PlayerSide side() const;
	
public:
	// State
	PieceState state() const;
	void setState(PieceState state);
	bool hasMoved() const;
	void reset();

public:
	// Moving
	SquarePosition position() const;
	void moveTo(SquarePosition square, bool instantly);
	virtual bool canAttack(SquarePosition square) const;
	virtual bool canMoveTo(SquarePosition square) const = 0;

public:
	// Resource
	virtual const std::string resourceId() const = 0;

public:
	// Selecting
	void select(bool enable);
	void highlight();

private:
	// Fields
	NumericId m_id;
	PlayerSide m_side;
	PieceState m_state;
	SquarePosition m_squarePosition;
	spSprite m_sprite;
	spTween m_selectionTween;
	bool m_hasMoved;
};

class King : public Piece
{
	virtual bool canMoveTo(SquarePosition square) const;
	virtual const std::string resourceId() const;
};

class Queen : public Piece
{
	virtual bool canMoveTo(SquarePosition square) const;
	virtual const std::string resourceId() const;
};

class Rook : public Piece
{
	virtual bool canMoveTo(SquarePosition square) const;
	virtual const std::string resourceId() const;
};

class Knight : public Piece
{
	virtual bool canMoveTo(SquarePosition square) const;
	virtual const std::string resourceId() const;
};

class Bishop : public Piece
{
	virtual bool canMoveTo(SquarePosition square) const;
	virtual const std::string resourceId() const;
};

class Pawn : public Piece
{
	virtual bool canMoveTo(SquarePosition square) const;
	virtual bool canAttack(SquarePosition square) const;
	virtual const std::string resourceId() const;
};