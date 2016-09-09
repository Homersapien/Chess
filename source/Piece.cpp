#include "Piece.h"

Piece::Piece() :
	m_id(0),
	m_side(PLAYER_SIDE_WHITE),
	m_state(PIECE_STATE_NORMAL),
	m_hasMoved(false),
	m_sprite(NULL)
{}

Piece::~Piece()
{
	m_sprite->detach();
}

void Piece::init(NumericId id, PlayerSide side, SquarePosition square)
{
	// Preconditions
	assert(id != 0);
	assert(m_id == 0);

	// Initialize parameters
	m_id = id;
	m_side = side;

	// Create a sprite
	m_sprite = new Sprite();
	m_sprite->setResAnim(Game::instance()->resources().getResAnim(resourceId() + (m_side == PLAYER_SIDE_WHITE ? "_w" : "_b")));
	m_sprite->setTouchEnabled(false);
	m_sprite->attachTo(board()->base());

	// Set the pivot to the center
	Vector2 pivot = m_sprite->getSize() / 2;
	pivot.x = float(int(pivot.x));
	pivot.y = float(int(pivot.y));
	m_sprite->setAnchorInPixels(pivot);
	
	// Move to the initial position
	moveTo(square, true);
	reset();
}

NumericId Piece::id() const
{
	return m_id;
}

Board* Piece::board() const
{
	return Game::instance()->scene()->board();
}

PlayerSide Piece::side() const
{
	return m_side;
}

PieceState Piece::state() const
{
	return m_state;
}

void Piece::setState(PieceState state)
{
	m_state = state;

	m_sprite->setVisible(m_state == PIECE_STATE_NORMAL);
}

bool Piece::hasMoved() const
{
	return m_hasMoved;
}

void Piece::reset()
{
	setState(PIECE_STATE_NORMAL);
	m_hasMoved = false;
}

SquarePosition Piece::position() const
{
	return m_squarePosition;
}

void Piece::moveTo(SquarePosition square, bool instantly)
{
	// Set the position
	spActor squareActor = board()->square(square);
	Vector2 centerPosition = squareActor->getPosition() + squareActor->getSize() / 2.0f;
	centerPosition.x = float(int(centerPosition.x));
	centerPosition.y = float(int(centerPosition.y));
	m_sprite->setPosition(centerPosition);

	m_squarePosition = square;
	m_hasMoved = true;
}

bool Piece::canAttack(SquarePosition square) const
{
	return canMoveTo(square);
}

void Piece::select(bool enable)
{
	if (enable)
	{
		m_selectionTween = m_sprite->addTween(Actor::TweenRotation(SELECTED_PIECE_ANIMATION_ANGLE), SELECTED_PIECE_ANIMATION_PERIOD, -1, true);
	}
	else
	{
		if (m_selectionTween)
		{
			m_sprite->removeTween(m_selectionTween);
			m_sprite->addTween(Actor::TweenRotation(0), PIECE_DESELECTION_DURATION);
		}
	}
}

void Piece::highlight()
{
	m_sprite->removeTweens();
	m_sprite->setRotation(0);
	m_sprite->addTween(Actor::TweenRotation(SELECTED_PIECE_ANIMATION_ANGLE / 2), SELECTED_PIECE_ANIMATION_PERIOD / 2, 1, true);
}

bool King::canMoveTo(SquarePosition square) const
{
	return std::max(abs(position().row - square.row), abs(position().col - square.col)) == 1;
}

const std::string King::resourceId() const { return "king"; }

bool Queen::canMoveTo(SquarePosition square) const
{
	int deltaRow = abs(position().row - square.row);
	int deltaCol = abs(position().col - square.col);
	
	return
		deltaRow == deltaCol && board()->diagonalPathIsFree(position(), square) ||
		deltaRow == 0 && board()->horizontalPathIsFree(position(), square) ||
		deltaCol == 0 && board()->verticalPathIsFree(position(), square);
}

const std::string Queen::resourceId() const { return "queen"; }

bool Rook::canMoveTo(SquarePosition square) const
{
	return
		position().row == square.row && board()->horizontalPathIsFree(position(), square) ||
		position().col == square.col && board()->verticalPathIsFree(position(), square);
}

const std::string Rook::resourceId() const { return "rook"; }

bool Knight::canMoveTo(SquarePosition square) const
{
	int deltaRow = abs(position().row - square.row);
	int deltaCol = abs(position().col - square.col);

	return
		deltaRow == 2 && deltaCol == 1 ||
		deltaRow == 1 && deltaCol == 2;
}

const std::string Knight::resourceId() const { return "knight"; }

bool Bishop::canMoveTo(SquarePosition square) const
{
	return
		abs(position().row - square.row) == abs(position().col - square.col) &&
		board()->diagonalPathIsFree(position(), square);
}

const std::string Bishop::resourceId() const { return "bishop"; }

bool Pawn::canMoveTo(SquarePosition square) const
{
	int forwardDirection = (side() == PLAYER_SIDE_BLACK ? 1 : -1);
	SquarePosition nextSquare = SquarePosition(position().row + forwardDirection, position().col);
	SquarePosition farSquare = SquarePosition(position().row + forwardDirection * 2, position().col);
	
	return
		square == nextSquare ||
		!hasMoved() && square == farSquare && board()->pieceOnSquare(nextSquare) == 0;
}

bool Pawn::canAttack(SquarePosition square) const
{
	int forwardDirection = (side() == PLAYER_SIDE_BLACK ? 1 : -1);

	return
		square.row == position().row + forwardDirection &&
		(square.col == position().col + 1 || square.col == position().col - 1);
}

const std::string Pawn::resourceId() const { return "pawn"; }
