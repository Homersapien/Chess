#include "Match.h"

Match::Match() :
	m_state(GAME_STATE_PLAYING),
	m_currentPlayer(PLAYER_SIDE_WHITE),
	m_indicatorNextUpdateTime(-1)
{}

Match::~Match()
{}

void Match::reset()
{
	Scene* scene = Game::instance()->scene();
	scene->gui()->reset();
	scene->board()->reset();
	m_currentPlayer = PLAYER_SIDE_WHITE;
	m_moveStartTime = getStage()->getClock()->getTime();
	m_state = GAME_STATE_PLAYING;
}

GameState Match::state() const
{
	return m_state;
}

PlayerSide Match::currentPlayer() const
{
	return m_currentPlayer;
};

timeMS Match::moveStartTime() const
{
	return m_moveStartTime;
}

void Match::makeMove(NumericId pieceId, SquarePosition targetPosition)
{
	Scene* scene = Game::instance()->scene();

	// Preconditions
	shpPiece piece = scene->board()->piece(pieceId);
	assert(piece->side() == m_currentPlayer);

	// Find an enemy piece
	NumericId enemyPieceId = scene->board()->pieceOnSquare(targetPosition);
	assert(enemyPieceId == 0 || scene->board()->piece(enemyPieceId)->side() != piece->side());

	// If the piece can move to the target position
	if (enemyPieceId == 0 && piece->canMoveTo(targetPosition) ||
		enemyPieceId != 0 && piece->canAttack(targetPosition))
	{
		// Capture an enemy piece
		if (enemyPieceId != 0)
		{
			shpPiece enemyPiece = scene->board()->piece(enemyPieceId);

			// The target piece must be an enemy
			assert(enemyPiece->side() != m_currentPlayer);

			enemyPiece->setState(PIECE_STATE_CAPTURED);
		}

		// Move to the target position
		piece->moveTo(targetPosition, false);

		// Reset selection
		scene->board()->selectPiece(0);

		// Change the current player
		m_currentPlayer = (m_currentPlayer == PLAYER_SIDE_WHITE ? PLAYER_SIDE_BLACK : PLAYER_SIDE_WHITE);
		m_moveStartTime = getStage()->getClock()->getTime();
		m_indicatorNextUpdateTime = -1;

		// Check win conditions
		checkWinConditions();
	}
	else
	{
		scene->board()->highlightPieceMoves(pieceId);
	}
}

void Match::checkWinConditions()
{
	PlayerSide enemySide = (m_currentPlayer == PLAYER_SIDE_WHITE ? PLAYER_SIDE_BLACK : PLAYER_SIDE_WHITE);
	if (kingIsInCheck(enemySide))
	{
		finish("Checkmate");
	}
	else if (!playerCanMakeMove(m_currentPlayer))
	{
		finish("Stalemate");
	}
}

bool Match::playerCanMakeMove(PlayerSide side) const
{
	Scene* scene = Game::instance()->scene();

	for (NumericId pieceId = 1; pieceId <= scene->board()->piecesNumber(); ++pieceId)
	{
		shpPiece piece = scene->board()->piece(pieceId);

		if (piece->state() == PIECE_STATE_NORMAL &&
			piece->side() == side)
		{
			for (int row = 0; row < BOARD_SIZE; ++row)
			{
				for (int col = 0; col < BOARD_SIZE; ++col)
				{
					SquarePosition position(row, col);
					if (position == piece->position())
					{
						continue;
					}

					NumericId otherPieceId = scene->board()->pieceOnSquare(position);

					if (piece->canMoveTo(position) && otherPieceId == 0 ||
						piece->canAttack(position) && otherPieceId != 0 && scene->board()->piece(otherPieceId)->side() != side)
					{
						log::messageln("%s %d can move from (%d, %d) to (%d, %d)",
							piece->resourceId().c_str(),
							piece->id(),
							piece->position().row + 1,
							piece->position().col + 1,
							position.row + 1,
							position.col + 1);

						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Match::kingIsInCheck(PlayerSide side) const
{
	Scene* scene = Game::instance()->scene();
	shpPiece king = scene->board()->piece(scene->board()->kingId(side));

	for (NumericId pieceId = 1; pieceId <= scene->board()->piecesNumber(); ++pieceId)
	{
		shpPiece piece = scene->board()->piece(pieceId);
		if (piece->state() == PIECE_STATE_NORMAL &&
			piece->side() != side &&
			piece->canAttack(king->position()))
		{
			log::messageln(
				"King is in check from %s %d (%d, %d)",
				piece->resourceId().c_str(),
				piece->id(),
				piece->position().row + 1,
				piece->position().col + 1);

			return true;
		}
	}

	return false;
}

void Match::update()
{
	// Get the current time
	timeMS currentTime = getStage()->getClock()->getTime();

	// Check the move time limit
	if (m_state == GAME_STATE_PLAYING &&
		currentTime > m_moveStartTime + MOVE_TIME_LIMIT)
	{
		finish("Time Is Over");
	}

	// Update time indicators
	if (currentTime >= m_indicatorNextUpdateTime)
	{
		m_indicatorNextUpdateTime = currentTime + INDICATORS_UPDATE_PERIOD;

		Game::instance()->scene()->board()->updateTimeIndicator(
			m_currentPlayer,
			MOVE_TIME_LIMIT - (currentTime - m_moveStartTime));
	}
}

void Match::finish(const std::string& reason)
{
	m_state = GAME_STATE_FINISHED;
	Game::instance()->scene()->gui()->showMessage(reason, "Press any key to restart the game...");
}