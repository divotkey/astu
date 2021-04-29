#pragma once

#include <iostream>

// Local includes
#include "SignalService.h"
#include "Events.h"
#include "Service.h"
#include "ServiceManager.h"

namespace astu {

    class KeystrokeListener : virtual public Service, private IKeystrokeListener {
    public:

        /**
         * Constructor.
         */
        KeystrokeListener() {
            AddStartupHook([this](){ ASTU_SERVICE(KeystrokeEventService).AddListener(this); });
            AddShutdownHook([this](){ ASTU_SERVICE(KeystrokeEventService).RemoveListener(this); });
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
