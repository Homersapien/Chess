#include "Scene.h"

Scene::Scene()
{
	// Add a background
	spSprite background = new Sprite();
	background->setResAnim(Game::instance()->resources().getResAnim("background"));
	background->setPosition((Vector2(core::getDisplaySize()) - background->getSize()) / 2.0f);
	background->attachTo(getStage());

	// Create a board
	m_board.reset(new Board());

	// Create GUI
	m_gui.reset(new Gui());
}

Scene::~Scene()
{}

Gui* Scene::gui()
{
	return m_gui.get();
}

Board* Scene::board()
{
	return m_board.get();
}