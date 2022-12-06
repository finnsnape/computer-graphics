#include "EventUtils.h"
#include "Scene.h"
#include <map>

std::map<SDL_Keycode, Scene::Mode> modeMap = {
        {SDLK_1, Scene::WIRE_FRAME},
        {SDLK_2, Scene::RASTERISED},
        {SDLK_3, Scene::RAY_TRACED},
        {SDLK_4, Scene::RAY_TRACED_SHADOWS}
};

namespace {
    void doOperation(SDL_Keycode key, Camera &camera, SDL_Event event) {
        switch(key) {
            case SDLK_w:
                camera.translate(Camera::y, 1);
                break;
            case SDLK_s:
                camera.translate(Camera::y, -1);
                break;
            case SDLK_a:
                camera.translate(Camera::x, -1);
                break;
            case SDLK_d:
                camera.translate(Camera::x, 1);
                break;
            case SDLK_MINUS:
                camera.translate(Camera::z, 1);
                break;
            case SDLK_EQUALS:
                camera.translate(Camera::z, -1);
                break;
            case SDLK_UP:
                camera.rotate(Camera::x, -1);
                break;
            case SDLK_DOWN:
                camera.rotate(Camera::x, 1);
                break;
            case SDLK_LEFT:
                camera.rotate(Camera::y, -1);
                break;
            case SDLK_RIGHT:
                camera.rotate(Camera::y, 1);
                break;
            case SDLK_o:
                camera.orbit = !camera.orbit;
                break;
            default:
                return;
        }
        //std::cout << "Doing operation: " << SDL_GetKeyName(key) << "..." << std::endl;
    }

    void changeMode(SDL_Keycode key, Scene &scene) {
        Scene::Mode newMode = modeMap[key];
        scene.mode = newMode;
        std::cout << "Setting mode to: " << newMode + 1 << "..." << std::endl;
    }
}

namespace EventUtils {
    void handleEvent(SDL_Event event, Scene &scene) {
        if (event.type != SDL_KEYDOWN) return;
        SDL_Keycode key = event.key.keysym.sym;
        if (modeMap.count(key)) {
            changeMode(key, scene); // wire frame, ray traced, etc.
        } else {
            doOperation(key, scene.camera, event); // rotate, translate
        }
    }
}