/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */


// Local includes
#include "SdlJoystickService.h"
#include "ISdlEventListener.h"
#include "SdlEventService.h"

// SDL 2 includes
#include <SDL2/SDL.h>

// C++ Standard Libryry includes
#include <stdexcept>
#include <iostream>
#include <vector>

using namespace std;

namespace astu {

    class EventListenerProxy : public ISdlEventListener {
    public:

        EventListenerProxy(InputMappingService& mapper);
        ~EventListenerProxy();

        // Inherited via ISdlEventListener
        virtual void HandleEvent(const SDL_Event & event);

    private:
        InputMappingService& mapper;
        SDL_GameController* gameController;
        map<int, SDL_GameController*> gameControllers;

        void AddGameController(int index);
        void RemoveGameController(int instanceId);
        bool HasGameController(int instanceId) const;
    };

    EventListenerProxy::EventListenerProxy(InputMappingService& mapper)
        : mapper(mapper)
    {
        SDL_JoystickEventState(SDL_ENABLE);
    }

    EventListenerProxy::~EventListenerProxy()
    {
        for (const auto& it: gameControllers) {
            if (it.second) {
                SDL_GameControllerClose(it.second);
            }
        }
        gameControllers.clear();
    }

    void EventListenerProxy::AddGameController(int index) {
        SDL_GameController* gameController = SDL_GameControllerOpen(index);
        if (!gameController) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "unable to open game controller: %s", SDL_GetError());

            return;
        }

        SDL_Joystick *joyStick = SDL_GameControllerGetJoystick(gameController);
        int instanceId = SDL_JoystickInstanceID(joyStick);

        assert(!HasGameController(instanceId));
        gameControllers[instanceId] = gameController;
    }

    void EventListenerProxy::RemoveGameController(int instanceId) {
        auto it = gameControllers.find(instanceId);
        if (it == gameControllers.end()) {
            return;
        }
        SDL_GameControllerClose(it->second);
        gameControllers.erase(it);
    }

    bool EventListenerProxy::HasGameController(int instanceId) const
    {
        return gameControllers.find(instanceId) != gameControllers.end();
    }


    void EventListenerProxy::HandleEvent(const SDL_Event & event) {
        // Uncomment for debugging.
        // cout << "got event: " << event.type << endl;

        switch (event.type) {
        case SDL_JOYDEVICEADDED:
            // Uncomment for debugging.
            // cout << "game controller added: " << event.jdevice.which << endl;
            AddGameController(event.jdevice.which);
            break;

        case SDL_JOYDEVICEREMOVED:
            // Uncomment for debugging.
            // cout << "game controller removed: " << event.jdevice.which << endl;
            RemoveGameController(event.jdevice.which);
            break;

        case SDL_JOYBALLMOTION:
            break;

        case SDL_JOYHATMOTION:
            break;

        case SDL_JOYBUTTONDOWN:
            break;

        case SDL_JOYBUTTONUP:
            break;

        case SDL_JOYAXISMOTION:
            // Uncomment for debugging.
            // cout << "game controller axis " << (int) event.jaxis.axis << ": " << event.jaxis.value << endl;
            break;
        }
    }

    SdlJoystickService::SdlJoystickService()
        : Service("SDL Joystick Service")
    {
        // Intentionally left empty.
    }

     SdlJoystickService::~SdlJoystickService()
     {
        // Intentionally left empty.
     }

    void SdlJoystickService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, 
            "Starting up SDL joystick service");

        if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "Couldn't initialize SDL events subsystem: %s", SDL_GetError());     
                  
            throw std::runtime_error(SDL_GetError());
        }

        inputMapperSrv = ASTU_GET_SERVICE_OR_NULL(InputMappingService);

        proxy = make_unique<EventListenerProxy>(*inputMapperSrv);
        ASTU_SERVICE(SdlEventService).AddSdlEventListener(*proxy);
    }

    void SdlJoystickService::OnShutdown() 
    {
        ASTU_SERVICE(SdlEventService).RemoveSdlEventListener(*proxy);
        proxy = nullptr;
        inputMapperSrv = nullptr;
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, 
            "Shutting down SDL joystick service");

        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }

} // end of namespace