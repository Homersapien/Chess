#include "Game.h"

Game* Game::s_instance = 0;

Game::Game() :
	m_scene()
{}

Game::~Game()
{}

Scene* Game::scene()
{
	return m_scene.get();
}

Match* Game::match()
{
	return m_match.get();
}

Game* Game::instance()
{
	if (!s_instance)
	{
		s_instance = new Game();
	}
	return s_instance;
}

int Game::mainloop()
{
	// Update engine-internal components
	// If input events are available, they are passed to Stage::instance.handleEvent
	// If the function returns true, it means that the user requested the application to terminate
	bool done = core::update();

	// It gets passed to our example game implementation
	update();

	// Update our stage
	// Update all actors. Actor::update will also be called for all its children
	getStage()->update();

	if (core::beginRendering())
	{
		Color clearColor(32, 32, 32, 255);
		Rect viewport(Point(0, 0), core::getDisplaySize());
		// Render all actors inside the stage. Actor::render will also be called for all its children
		getStage()->render(clearColor, viewport);

		core::swapDisplayBuffers();
	}

	return done ? 1 : 0;
}

// Application entry point
void Game::run()
{
	ObjectBase::__startTracingLeaks();

	// Initialize Oxygine's internal stuff
	core::init_desc desc;
	desc.title = "Oxygine Application";

#if OXYGINE_SDL || OXYGINE_EMSCRIPTEN
	// The initial window size can be set up here on SDL builds
	desc.w = 1280;
	desc.h = 720;
	// Marmalade settings can be modified from the emulator's menu
#endif


	preinit();
	core::init(&desc);


	// Create the stage. Stage is a root node for all updateable and drawable objects
	Stage::instance = new Stage(true);
	Point size = core::getDisplaySize();
	getStage()->setSize(size);

	// DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
	DebugActor::show();

	// Initializes our example game. See example.cpp
	init();

#ifdef EMSCRIPTEN
	/*
	If you build for Emscripten, mainloop is called automatically and shouldn't be called here.
	See emscripten_set_main_loop in the EMSCRIPTEN section below
	*/
	return;
#endif

#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
	// On iPhone mainloop is called automatically by CADisplayLink, see int main() below
	return;
#endif

	// This is the main game loop.
	while (1)
	{
		int done = mainloop();
		if (done)
			break;
	}
	/*
	If we get here, the user has requested the Application to terminate.
	We dump and log all our created objects that have not been freed yet
	*/
	ObjectBase::dumpCreatedObjects();

	/*
	Let's clean up everything right now and call ObjectBase::dumpObjects() again.
	We need to free all allocated resources and delete all created actors.
	All actors/sprites are smart-pointer objects and don't need to be removed by hand.
	But now we want to delete it by hand.
	*/

	// See example.cpp for the shutdown function implementation
	destroy();


	//renderer.cleanup();

	// Releases all internal components and the stage
	core::release();

	// The dump list should be empty by now,
	// we want to make sure that there aren't any memory leaks, so we call it again.
	ObjectBase::dumpCreatedObjects();

	ObjectBase::__stopTracingLeaks();
	//end
}

void Game::preinit()
{}

void Game::init()
{
	// Load XML file with resources definition
	m_resources.loadXML(RESOURCES_DEFINITION_FILE_PATH);

	// Create a scene
	m_scene.reset(new Scene());

	// Start the first move
	m_match.reset(new Match());
	m_match->reset();

	// Assign event handlers
	oxygine::core::getDispatcher()->addEventListener(oxygine::core::EVENT_SYSTEM, CLOSURE(this, &Game::onSystemEvent));
}

void Game::destroy()
{
	//board.free();
	//res.free();

	m_scene.release();
}
void Game::update()
{
	m_match->update();
}

void Game::onSystemEvent(Event* e)
{
	SDL_Event *sdlEvent = (SDL_Event*)e->userData;

	// Handle a keyboard event
	if (sdlEvent->type == SDL_KEYDOWN)
	{
		switch (sdlEvent->key.keysym.sym)
		{
			case SDLK_ESCAPE:
				core::requestQuit();
				break;
			
			default:
				if (m_match->state() == GAME_STATE_FINISHED)
				{
					m_match->reset();
				}
				break;
		}
	}
}

Resources& Game::resources()
{
	return m_resources;
}