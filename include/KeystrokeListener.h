#pragma once

#include <iostream>

// Local includes
#include "SignalService.h"
#include "Events.h"
#include "Service.h"
#include "ServiceManager.h"

namespace astu {

    class KeystrokeListener : private IKeystrokeListener, private IServiceListener {
    public:

        KeystrokeListener() {
            ServiceManager::GetInstance().GetService<ServiceEventService>().AddListener(this);
        }

        virtual ~KeystrokeListener() {
            ServiceManager::GetInstance().GetService<ServiceEventService>().RemoveListener(this);
        }

        virtual void OnSignal(const ServiceEvent & event) override {

            if (dynamic_cast<KeystrokeListener*>(&event.service) == this) {

                auto & keystrokeSrv = ServiceManager::GetInstance().GetService<KeystrokeEventService>();
                switch (event.type) {
                case ServiceEvent::Started:
                    keystrokeSrv.AddListener(this);
                    break;

                case ServiceEvent::Stopped:
                    keystrokeSrv.RemoveListener(this);
                    break;
                }
            }
        }

    protected:

        /** 
         * Called by this base class when a key has been pressed.
         * 
         * @param keycode   the code of the key
         */
        virtual void OnKeyPressed(int keycode) {}

        /** 
         * Called by this base class when a key has been released.
         * 
         * @param keycode   the code of the key
         */
        virtual void OnKeyReleased(int keycode) {}

    private:

        // Inherited via KeystrokeListener
        virtual void OnSignal(const KeystrokeEvent & signal) override {
            if (signal.pressed) {
                OnKeyPressed(signal.keycode);
            } else {
                OnKeyReleased(signal.keycode);
            }
        }

    };

} // end of namespace
