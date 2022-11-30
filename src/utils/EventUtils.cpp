#include "EventUtils.h"
#include "Scene.h"
#include <map>

std::map<SDL_Keycode, Mode> modeMap = {
        {SDLK_1, WIRE_FRAME},
        {SDLK_2, RASTERISED},
        {SDLK_3, RAY_TRACED},
        {SDLK_4, RAY_TRACED_SHADOWS}
};

namespace {
    void changeMode(SDL_Keycode key, Scene &scene) {
        Mode newMode = modeMap[key];
        scene.mode = newMode;
        std::cout << "Setting mode to: " << newMode + 1 << "..." << std::endl;
    }
}

namespace EventUtils {
    void handleEvent(SDL_Event event, Scene &scene) {
        if (event.type != SDL_KEYDOWN) return;
        SDL_Keycode key = event.key.keysym.sym;
        if (modeMap.count(key)) {
            changeMode(key, scene);
        }
    }
}