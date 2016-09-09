#pragma once

#include "Common.h"

// Pathes
extern const std::string RESOURCES_DEFINITION_FILE_PATH;

// Enums
enum PlayerSide
{
	PLAYER_SIDE_WHITE,
	PLAYER_SIDE_BLACK
};

enum GameState
{
	GAME_STATE_PLAYING,
	GAME_STATE_FINISHED
};

// Board constants
const int BOARD_SIZE = 8;
const timeMS SQUARE_HIGHLIGHT_DURATION = 500;

// Piece constants
const float SELECTED_PIECE_ANIMATION_ANGLE = 0.5f;
const timeMS SELECTED_PIECE_ANIMATION_PERIOD = 500;
const timeMS PIECE_DESELECTION_DURATION = 250;

// GUI constants
const unsigned char OVERLAY_OPACITY = 192;

// Colors
const Color TEXT_COLOR_NORMAL = Color(255, 255, 255);
const Color TEXT_COLOR_SUBDUED = Color(220, 220, 220);

// Match constants
const timeMS MOVE_TIME_LIMIT = 60000;
const timeMS INDICATORS_UPDATE_PERIOD = 500;