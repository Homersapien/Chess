#pragma once

#include "Rules.h"
#include "Square.h"
#include "Scene.h"
#include "Match.h"

class Game : public Actor, private Uncopyable
{
protected:
	// Singleton
	Game();
	virtual ~Game();
	static Game* s_instance;

public:
	// Singleton instance
	static Game* instance();

public:
	// Starting
	void run();
	int mainloop();
	
public:
	// Resources
	Resources& resources();

public:
	// Objects
	Scene* scene();
	Match* match();

protected:
	// Implementation (events)
	void preinit();
	void init();
	void destroy();
	void update();

protected:
	// Implementation (event handling)
	void onSystemEvent(Event* e);

private:
	// Fields
	Resources m_resources;
	upScene m_scene;
	upMatch m_match;
};

