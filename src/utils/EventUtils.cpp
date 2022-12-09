#include "EventUtils.h"
#include "Scene.h"
#include <map>

std::map<SDL_Keycode, Scene::RenderMode> renderModeMap = {
        {SDLK_1, Scene::WIRE_FRAME},
        {SDLK_2, Scene::RASTERISED},
        {SDLK_3, Scene::RAY_TRACED},
};

std::map<SDL_Keycode, Scene::LightingMode> lightingModeMap = {
        {SDLK_4, Scene::DEFAULT},
        {SDLK_5, Scene::HARD_SHADOWS},
        {SDLK_6, Scene::PROXIMITY},
        {SDLK_7, Scene::ANGLE_OF_INCIDENCE},
        {SDLK_8, Scene::AMBIENT},
        {SDLK_9, Scene::SPECULAR},
};

namespace {
    void doOperation(SDL_Keycode key, Scene &scene, SDL_Event event) {
        switch(key) {
            case SDLK_w:
                scene.camera.translate(Camera::y, 1);
                break;
            case SDLK_s:
                scene.camera.translate(Camera::y, -1);
                break;
            case SDLK_a:
                scene.camera.translate(Camera::x, -1);
                break;
            case SDLK_d:
                scene.camera.translate(Camera::x, 1);
                break;
            case SDLK_MINUS:
                scene.camera.translate(Camera::z, 1);
                break;
            case SDLK_EQUALS:
                scene.camera.translate(Camera::z, -1);
                break;
            case SDLK_UP:
                scene.camera.rotate(Camera::x, -1);
                break;
            case SDLK_DOWN:
                scene.camera.rotate(Camera::x, 1);
                break;
            case SDLK_LEFT:
                scene.camera.rotate(Camera::y, -1);
                break;
            case SDLK_RIGHT:
                scene.camera.rotate(Camera::y, 1);
                break;
            case SDLK_o:
                scene.camera.orbit = !scene.camera.orbit;
                break;
            case SDLK_h:
                scene.moveLight(Camera::Axis::y, 1);
                break;
            case SDLK_n:
                scene.moveLight(Camera::Axis::y, -1);
                break;
            case SDLK_b:
                scene.moveLight(Camera::Axis::x, -1);
                break;
            case SDLK_m:
                scene.moveLight(Camera::Axis::x, 1);
                break;
            case SDLK_j:
                scene.moveLight(Camera::Axis::z, 1);
                break;
            case SDLK_k:
                scene.moveLight(Camera::Axis::z, -1);
                break;
            default:
                return;
        }
        //std::cout << "Doing operation: " << SDL_GetKeyName(key) << "..." << std::endl;
    }

    void changeRenderMode(SDL_Keycode key, Scene &scene) {
        Scene::RenderMode newMode = renderModeMap[key];
        scene.renderMode = newMode;
        std::cout << "Setting render mode to: " << newMode + 1 << "..." << std::endl;
    }

    void changeLightingMode(SDL_Keycode key, Scene &scene) {
        Scene::LightingMode newMode = lightingModeMap[key];
        scene.renderMode = Scene::RAY_TRACED;
        scene.lightingMode = newMode;
        std::cout << "Setting lighting mode to: " << newMode + 4 << "..." << std::endl;
    }
}

namespace EventUtils {
    void handleEvent(SDL_Event event, Scene &scene) {
        if (event.type != SDL_KEYDOWN) return;
        SDL_Keycode key = event.key.keysym.sym;
        if (renderModeMap.count(key)) {
            changeRenderMode(key, scene); // wire frame, ray traced, etc.,
        } else if (lightingModeMap.count(key)) {
            changeLightingMode(key, scene); // hard shadows, proximity, etc.,
        } else {
            doOperation(key, scene, event); // rotate, translate, etc.,
        }
        scene.draw();
    }
}