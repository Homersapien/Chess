#pragma once

#include "Common.h"

class Scene;
typedef std::unique_ptr<Scene> upScene;

#include "Gui.h"
#include "Board.h"

class Scene : private Uncopyable
{
public:
	// Initialization
	Scene();
	virtual ~Scene();

public:
	// Objects
	Gui* gui();
	Board* board();

private:
	// Fields
	upBoard m_board;
	upGui m_gui;
};
