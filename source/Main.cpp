/**
    Attention!
    This file initializes the Oxygine engine.
    If you just started here and don't understand the code completely, feel free to come back later.
    You can start from example.cpp and example.h, which main functions are called from here.
*/
#include "core/oxygine.h"
#include "Stage.h"
#include "DebugActor.h"
#include "Game.h"

using namespace oxygine;

#ifdef __S3E__
int main(int argc, char* argv[])
{
    run();
    return 0;
}
#endif

#ifdef OXYGINE_SDL

#include "SDL_main.h"
#include "SDL.h"

extern "C"
{
    void one(void* param) { Game::instance()->mainloop(); }
	void oneEmsc() { Game::instance()->mainloop(); }

    int main(int argc, char* argv[])
    {
		Game::instance()->run();

#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
        // If parameter 2 is set to 1, refresh rate will be 60 fps, 2 - 30 fps, 3 - 15 fps.
        SDL_iPhoneSetAnimationCallback(core::getWindow(), 1, one, nullptr);
#endif

#if EMSCRIPTEN
        emscripten_set_main_loop(oneEmsc, 0, 0);
#endif

        return 0;
    }
};
#endif