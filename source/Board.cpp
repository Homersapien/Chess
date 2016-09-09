#include "Board.h"

Board::Board() :
	m_selectedPieceId(0),
	m_whiteKingId(0),
	m_blackKingId(0),
	m_nextPieceId(1)
{
	// Create a base
	m_base = new Sprite();
	m_base->setResAnim(Game::instance()->resources().getResAnim("board"));
	
	// Set the base position
	Vector2 basePosition = getStage()->getSize() / 2.0f - m_base->getSize() / 2.0f;
	basePosition.x = float(int(basePosition.x));
	basePosition.y = float(int(basePosition.y));
	m_base->setPosition(basePosition);
	m_base->attachTo(getStage());

	// Create squares
	ResAnim* squareAnimW = Game::instance()->resources().getResAnim("board_square_w");
	ResAnim* squareAnimB = Game::instance()->resources().getResAnim("board_square_b");
	Vector2 squareSize = squareAnimW->getSize();
	Vector2 squaresPadding = (m_base->getSize() - squareSize * BOARD_SIZE) / 2.0f;

	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			int squareIndex = row * BOARD_SIZE + col;
			m_squares[squareIndex].position = SquarePosition(row, col);
			m_squares[squareIndex].sprite = new Sprite();
			
			spSprite& square = m_squares[squareIndex].sprite;
			square->setResAnim((squareIndex - row) % 2 == 0 ? squareAnimW : squareAnimB);
			square->setPosition(Vector2(col * squareSize.x, row * squareSize.y) + squaresPadding);
			square->attachTo(m_base);
			
			square->setUserData(&m_squares[squareIndex]);
			square->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Board::onSquareClick));
		}
	}

	// Create a time indicator
	TextStyle timeIndicatorStyle;
	timeIndicatorStyle.font = Game::instance()->resources().getResFont("arial_small")->getFont();
	timeIndicatorStyle.color = TEXT_COLOR_SUBDUED;
	timeIndicatorStyle.vAlign = TextStyle::VALIGN_MIDDLE;
	timeIndicatorStyle.hAlign = TextStyle::HALIGN_CENTER;
	timeIndicatorStyle.fontSize = 14;

	m_timeIndicator = new TextField();
	m_timeIndicator->attachTo(m_base);
	m_timeIndicator->setVisible(false);
	m_timeIndicator->setStyle(timeIndicatorStyle);

	m_whiteIndicatorPosition = Vector2(m_base->getSize().x / 2.0f, m_base->getSize().y - squaresPadding.y / 2.0f - 1.0f);
	m_blackIndicatorPosition = Vector2(m_base->getSize().x / 2.0f, squaresPadding.y / 2.0f - 1.0f);
}

void Board::reset()
{
	// Clear the container
	m_pieces.clear();

	// Spawn pieces
	respawnPieces();
}

Board::~Board()
{
	m_base->detach();
}

shpPiece Board::piece(NumericId id) const
{
	// Preconditions
	assert(0 < id && id < m_pieces.size() + 1);

	return m_pieces[id - 1];
}

size_t Board::piecesNumber() const
{
	return m_pieces.size();
}

NumericId Board::kingId(PlayerSide side) const
{
	return (side == PLAYER_SIDE_WHITE ? m_whiteKingId : m_blackKingId);
}

NumericId Board::pieceOnSquare(SquarePosition square) const
{
	NumericId foundId = 0;
	for (size_t index = 0; index < m_pieces.size(); ++index)
	{
		if (m_pieces[index]->position() == square && m_pieces[index]->state() == PIECE_STATE_NORMAL)
		{
			foundId = index + 1;
			break;
		}
	}

	return foundId;
}

bool Board::diagonalPathIsFree(SquarePosition start, SquarePosition end) const
{
	// Preconditions
	assert(abs(start.row - end.row) == abs(start.col - end.col));

	if (start != end)
	{
		// Get the step value
		SquarePosition step(sign(end.row - start.row), sign(end.col - start.col));
		assert(abs(step.row) == 1 && abs(step.col) == 1);

		// Check squares
		for (SquarePosition position = start + step; position != end; position += step)
		{
			if (pieceOnSquare(position) != 0)
			{
				return false;
			}
		}
	}

	return true;
}

bool Board::horizontalPathIsFree(SquarePosition start, SquarePosition end) const
{
	// Preconditions
	assert(start.row == end.row);

	// Check squares
	for (SquarePosition position(start.row, std::min(start.col, end.col) + 1);
		position.col < std::max(start.col, end.col);
		++position.col)
	{
		if (pieceOnSquare(position) != 0)
		{
			return false;
		}
	}

	return true;
}

bool Board::verticalPathIsFree(SquarePosition start, SquarePosition end) const
{
	// Preconditions
	assert(start.col == end.col);

	// Check squares
	for (SquarePosition position(std::min(start.row, end.row) + 1, start.col);
		position.row < std::max(start.row, end.row);
		++position.row)
	{
		if (pieceOnSquare(position) != 0)
		{
			return false;
		}
	}

	return true;
}



void Board::highlightPieceMoves(NumericId pieceId)
{
	shpPiece piece = this->piece(pieceId);
	
	// For every square
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			// Find an enemy piece
			SquarePosition position = SquarePosition(row, col);
			NumericId enemyPieceId = pieceOnSquare(position);

			// If the square is not occupied by an ally
			if (enemyPieceId == 0 || this->piece(enemyPieceId)->side() != piece->side())
			{
				// If the square is available
				if (enemyPieceId == 0 && piece->canMoveTo(position) ||
					enemyPieceId != 0 && piece->canAttack(position))
				{
					// Highlight the square
					int squareIndex = row * BOARD_SIZE + col;
					spSprite sprite = m_squares[squareIndex].sprite;
					sprite->removeTweens();
					sprite->setColor(Color::White);
					sprite->addTween(Sprite::TweenColor(Color::Red), SQUARE_HIGHLIGHT_DURATION, 1, true);
				}
			}
		}
	}
}

void Board::highlightPieces(PlayerSide side)
{
	for (size_t index = 0; index < m_pieces.size(); ++index)
	{
		if (m_pieces[index]->side() == side)
		{
			m_pieces[index]->highlight();
		}
	}
}

spActor Board::base()
{
	return m_base;
}

spActor Board::square(SquarePosition position)
{
	// Preconditions
	assert(0 <= position.row && position.row < BOARD_SIZE);
	assert(0 <= position.col && position.col < BOARD_SIZE);

	return m_squares[position.row * BOARD_SIZE + position.col].sprite;
}

void Board::selectPiece(NumericId id)
{
	// Preconditions
	assert(0 <= id && id < m_pieces.size() + 1);

	if (id == m_selectedPieceId)
		return;

	// Deselect the old piece
	if (m_selectedPieceId != 0)
	{
		m_pieces[m_selectedPieceId - 1]->select(false);
	}

	// Select the new piece
	m_selectedPieceId = id;
	if (m_selectedPieceId != 0)
	{
		m_pieces[m_selectedPieceId - 1]->select(true);
	}
}

NumericId Board::selectedPieceId() const
{
	return m_selectedPieceId;
}

void Board::respawnPieces()
{
	// Reset the next piece ID
	m_nextPieceId = 1;

	// Spawn the both sides
	spawnSidePieces(PLAYER_SIDE_WHITE);
	spawnSidePieces(PLAYER_SIDE_BLACK);

	// Postconditions
	assert(m_whiteKingId != 0);
	assert(m_blackKingId != 0);
}

void Board::spawnSidePieces(PlayerSide side)
{
	int firstRow = (side == PLAYER_SIDE_BLACK ? 0 : BOARD_SIZE - 1);
	int secondRow = firstRow + (side == PLAYER_SIDE_BLACK ? 1 : -1);

	// Create first row pieces
	spawnPiece(new Rook(),   side, SquarePosition(firstRow, 0));
	spawnPiece(new Knight(), side, SquarePosition(firstRow, 1));
	spawnPiece(new Bishop(), side, SquarePosition(firstRow, 2));
	spawnPiece(new Queen(),  side, SquarePosition(firstRow, 3));
	(side == PLAYER_SIDE_WHITE ? m_whiteKingId : m_blackKingId) = m_nextPieceId;
	spawnPiece(new King(),   side, SquarePosition(firstRow, 4));
	spawnPiece(new Bishop(), side, SquarePosition(firstRow, 5));
	spawnPiece(new Knight(), side, SquarePosition(firstRow, 6));
	spawnPiece(new Rook(),   side, SquarePosition(firstRow, 7));

	// Create pawns
	for (int index = 0; index < BOARD_SIZE; ++index)
	{
		spawnPiece(new Pawn(), side, SquarePosition(secondRow, index));
	}
}

void Board::spawnPiece(Piece* piece, PlayerSide side, SquarePosition position)
{
	m_pieces.push_back(shpPiece(piece));

	piece->init(
		m_nextPieceId,
		side,
		position);

	++m_nextPieceId;
}

void Board::onSquareClick(Event* e)
{
	// Get the clicked square
	const Square* square = static_cast<const Square*>(e->target->getUserData());

	// Find a piece on the square
	NumericId pieceId = pieceOnSquare(square->position);

	if (pieceId != 0 && piece(pieceId)->side() == Game::instance()->match()->currentPlayer())
	{
		// Select a piece
		selectPiece(pieceId);
	}
	else if (m_selectedPieceId != 0)
	{
		// Move the selected piece
		Game::instance()->match()->makeMove(m_selectedPieceId, square->position);
	}
	else
	{
		// Show possible moves
		highlightPieces(Game::instance()->match()->currentPlayer());
	}
}

void Board::updateTimeIndicator(PlayerSide side, timeMS time)
{
	// Set the color
	TextStyle style(m_timeIndicator->getStyle());
	style.color = (side == PLAYER_SIDE_WHITE ? Color::White : Color::Black);
	m_timeIndicator->setStyle(style);
	
	// Set the value
	char indicatorString[16];
	safe_sprintf(indicatorString, "%d", time / 1000);
	m_timeIndicator->setText(indicatorString);
	
	// Set the position
	m_timeIndicator->setPosition(side == PLAYER_SIDE_WHITE ? m_whiteIndicatorPosition : m_blackIndicatorPosition);

	// Set the visibility
	m_timeIndicator->setVisible(Game::instance()->match()->state() == GAME_STATE_PLAYING);
}