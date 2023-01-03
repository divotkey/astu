/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */


// Local includes
#include "SuiteSDL/SdlJoystickService.h"
#include "SuiteSDL/ISdlEventListener.h"
#include "SuiteSDL/SdlEventService.h"
#include "Input/Keys.h"

// SDL 2 includes
#include <SDL2/SDL.h>

// C++ Standard Libryry includes
#include <stdexcept>
#include <iostream>
#include <vector>
#include <limits>

using namespace std;

namespace astu {

    class EventListenerProxy : public ISdlEventListener {
    public:

        EventListenerProxy(InputMappingService& mapper, const map<Key, float> &deadzones);
        ~EventListenerProxy();

        // Inherited via ISdlEventListener
        virtual void HandleEvent(const SDL_Event & event);

    private:
        const map<Key, float> &deadzones;
        InputMappingService& mapper;
        SDL_GameController* gameController;
        map<int, SDL_GameController*> gameControllers;

        void AddGameController(int index);
        void RemoveGameController(int instanceId);
        bool HasGameController(int instanceId) const;

        const Key& TranslateButton(int sdlGamepadButton);
        const Key& TranslateAxis(int sdlGamepadAxis);
        float TranslateAxisValue(int16_t rawValue, float deadzone);
        float TranslateAxisValueFullRange(int16_t rawValue, float deadzone);

        float GetAxisDeadzone(const Key& key) const;
    };

    EventListenerProxy::EventListenerProxy(
        InputMappingService& mapper, 
        const map<Key, float> &deadzones
    )
        : mapper(mapper)
        , deadzones(deadzones)
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

    const Key& EventListenerProxy::TranslateButton(int sdlGamepadButton)
    {
        switch (sdlGamepadButton) {
            case SDL_CONTROLLER_BUTTON_A:
                return Keys::GamepadFaceButtonBottom;

            case SDL_CONTROLLER_BUTTON_B:
                return Keys::GamepadFaceButtonRight;

            case SDL_CONTROLLER_BUTTON_X:
                return Keys::GamepadFaceButtonLeft;

            case SDL_CONTROLLER_BUTTON_Y:
                return Keys::GamepadFaceButtonTop;

            default:
                return Keys::Unknown;
            }
    }

    const Key& EventListenerProxy::TranslateAxis(int sdlGamepadAxis)
    {
        switch (sdlGamepadAxis) {

            case SDL_CONTROLLER_AXIS_LEFTX:
                return Keys::GamepadLeftThumbstickX;

            case SDL_CONTROLLER_AXIS_LEFTY:
                return Keys::GamepadLeftThumbstickY;

            case SDL_CONTROLLER_AXIS_RIGHTX:
                return Keys::GamepadRightThumbstickX;

            case SDL_CONTROLLER_AXIS_RIGHTY:
                return Keys::GamepadRightThumbstickY;

            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                return Keys::GamepadLeftShoulder;

            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                return Keys::GamepadRightShoulder;

            default:
                return Keys::Unknown;
            }
    }

    float EventListenerProxy::GetAxisDeadzone(const Key& key) const
    {
        auto it = deadzones.find(key);
        if (it != deadzones.end()) {
            return it->second;
        } else {
            return 0.0f;
        }
    }

    float EventListenerProxy::TranslateAxisValue(int16_t rawValue, float deadzone)
    {
        float value;
        if (rawValue >= 0) {
            value = static_cast<float>(rawValue) / numeric_limits<int16_t>::max();
        } else {
            value = -static_cast<float>(rawValue) / numeric_limits<int16_t>::min();
        }

        if (value < deadzone && value > -deadzone) {
            return 0.0f;
        }

        return value;
    }

    float EventListenerProxy::TranslateAxisValueFullRange(int16_t rawValue, float deadzone)
    {
        float value;
        if (rawValue >= 0) {
            value = static_cast<float>(rawValue) / numeric_limits<int16_t>::max();
        } else {
            value = -static_cast<float>(rawValue) / numeric_limits<int16_t>::min();
        }

        value = (value + 1.0f) * 0.5f;

        if (value < deadzone) {
            return 0.0f;
        }
        
        return value;
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
            // Not supported at the moment.
            break;

        case SDL_JOYHATMOTION:
            // Not supported at the moment.
            break;

        case SDL_JOYBUTTONDOWN:
            mapper.ProcessKey(TranslateButton(event.jbutton.button), true);
            break;

        case SDL_JOYBUTTONUP:
            mapper.ProcessKey(TranslateButton(event.jbutton.button), false);
            break;

        case SDL_JOYAXISMOTION:
            // Uncomment for debugging.
            // cout << "game controller axis " << (int) event.jaxis.axis 
            //     << ": " << event.jaxis.value << " float: " 
            //     << TranslateAxisValue(event.jaxis.value) <<endl;

            {
                const Key& key = TranslateAxis(event.jaxis.axis);
                switch (event.jaxis.axis) {

                case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                    mapper.ProcessAxis(
                        TranslateAxis(event.jaxis.axis), 
                        TranslateAxisValueFullRange(event.jaxis.value, GetAxisDeadzone(key)));
                    break;

                default:
                    mapper.ProcessAxis(
                        TranslateAxis(event.jaxis.axis), 
                        TranslateAxisValue(event.jaxis.value, GetAxisDeadzone(key)));
                    break;
                }
            }
            break;
        }
    }

    SdlJoystickService::SdlJoystickService(float defaultDeadZone)
        : Service("SDL Joystick Service")
    {
        // Intentionally left empty.
        SetAxisDeadzone(Keys::GamepadLeftThumbstickX, defaultDeadZone);
        SetAxisDeadzone(Keys::GamepadLeftThumbstickY, defaultDeadZone);
        SetAxisDeadzone(Keys::GamepadRightThumbstickX, defaultDeadZone);
        SetAxisDeadzone(Keys::GamepadRightThumbstickY, defaultDeadZone);
    }

     SdlJoystickService::~SdlJoystickService()
     {
        // Intentionally left empty.
     }

    void SdlJoystickService::SetAxisDeadzone(const Key& key, float deadzone)
    {
        if (deadzone < 0.0f || deadzone > 1.0f) {
            throw std::domain_error("Dead zone out of range");
        }
        deadzones[key] = deadzone;
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

        proxy = make_unique<EventListenerProxy>(*inputMapperSrv, deadzones);
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