/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "UpdateService.h"
#include "Util/ListenerManager.h"

// C++ Standard Library includes
#include <array>
#include <vector>
#include <utility>

namespace astu {

    /**
     * A template signal listeners of a certain type.
     * 
     * **Example**
     * 
     * This example shows a signal listener which is listening to `int` values.
     * 
     * 
     * Header file: *MyListener.h*
     * ```
     * #pragma once
     * 
     * #include <SignalService.h>
     * 
     * class MyListener : public astu::ISignalListener<int> {
     * public:
     * 
     *   // Inheritred via ISignalListener
     *   virtual void OnSignal(const int & signal) override;    
     * };
     * ```
     *
     * Implementation file: *MyListener.cpp*
     * 
     * ```
     * #include <iostream>
     * #include "MyListener.h"
     * 
     * MyListener::void OnSignal(const int & signal)
     * {
     *   std::cout << "received signal " << signal << std::endl;
     * }    
     * ```
     * 
     * @tparam T    the type of signal this listeners is receiving
     * @ingroup srv_group
     */
    template <typename T> 
    class ISignalListener  {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~ISignalListener() {}

        /**
         * Called when a signal should be processed by this listener.
         *
         * The return value determines whether the signal has been consumed.
         * A consumed signal will no longer be forwarded to other signal
         * listeners.    
         * 
         * @param signal	the signal
         * @return `true` if the signal has been consumed
         */
        virtual bool OnSignal(const T & signal) = 0;        
    };

    /**
     * A template-based service which is used to transmit objects 
     * called "signals" to registered listeners.
     * 
     * This template class is declared final and hence cannot be used
     * as base class for other services.
     * 
     * **Example**
     * 
     * This example adds a new signal service to the service manager, which
     * transmits objects of type std::string.
     * 
     * ```
     * ASTU_CREATE_AND_ADD_SERVICE(SignalService<std::string>);
     * ```
     * 
     * To transmit signals of type string the following example code can be
     * used:
     * 
     * ```
     * ASTU_SERVICE(SignalService<std::string>).QueueSignal("This is a signal");
     * ```
     * 
     * @ingroup srv_group
     */
    template <typename T>
    class SignalService final : virtual public Service, private Updatable
    {
    public:

        /**
         * Constructor.
         * 
         * @param name      the name of this service
         * @param priority  the update priority of this service
         */
        SignalService(const std::string & name = "Signal Service", int priority = Normal)
            : Service(name)
            , Updatable(priority)
            , addQueue(&signalQueues[0])
            , sendQueue(&signalQueues[1])
        {
            // Intentionally left empty.
        }

        /**
         * Enqueues a signal for delayed transmission.
         * 
         * The specified signal will be queued and transmitted
         * during the next update cycle.
         *
         * @param signal    the signal to transmit to the registered listeners
         */
        void QueueSignal(const T & signal) {
            addQueue->push_back(signal);
        }

        /**
         * Fires a signal immediately.
         * 
         * The signal is transmitted to signal listener immediately.
         *
         * @param signal the signal to transmit to the registered receivers
         */
        void FireSignal(const T & signal) {
            listenerManager.VisitListeners([signal](ISignalListener<T> & listener) {
                return listener.OnSignal(signal);
            });

            rawListenerManager.VisitListeners([signal](ISignalListener<T> & listener) {
                return listener.OnSignal(signal);
            });
        }

        /**
         * Adds a signal listener to this service.
         * 
         * @param listener  the listener to add
         */
        void AddListener(const std::shared_ptr<ISignalListener<T>> & listener) {
            listenerManager.AddListener(listener);
        }

        /**
         * Adds a signal listener to this service.
         * 
         * @param listener  the the listener to add
         */
        void AddListener(ISignalListener<T> & listener) {
            rawListenerManager.AddListener(&listener);
        }

        /**
         * Removes a signal listener from this service.
         * 
         * @param listener  the listener to remove
         */
        void RemoveListener(const std::shared_ptr<ISignalListener<T>> & listener) {
            listenerManager.RemoveListener(listener);        
        }

        /**
         * Removes a signal listener from this service.
         * 
         * @param listener  the listener to remove
         */
        void RemoveListener(ISignalListener<T> & listener) {
            rawListenerManager.RemoveListener(&listener);        
        }

        /**
         * Tests whether a signal listener has already been added.
         * 
         * @param listener  the listener to test
         */
        bool HasListener(const std::shared_ptr<ISignalListener<T>> & listener) const {
            return listenerManager.HasListener(listener);
        }

        /**
         * Tests whether a signal listener has already been added.
         * 
         * @param listener  the listener to test
         */
        bool HasListener(const ISignalListener<T> & listener) const {
            return rawListenerManager.HasListener(&listener);
        }

    private:
        using SignalQueue = std::vector<T>;

        /** The signal queues. */
        std::array<SignalQueue, 2> signalQueues;

        /** The signal queue where to add new signals. */
        SignalQueue * addQueue;

        /** The signal queue used to transmit signals. */
        SignalQueue * sendQueue;

        /** Used to manage signal listeners. */
        ListenerManager<ISignalListener<T>> listenerManager;

        /** Used to manage signal listeners, stored by raw pointers. */
        RawListenerManager<ISignalListener<T>> rawListenerManager;


        // Inherited via Service
        virtual void OnShutdown() override {
            // ClearVariables pending signals and try to free memory used by queues.
            for (auto & queue : signalQueues) {
                queue.clear();
            }

            listenerManager.RemoveAllListeners();
            rawListenerManager.RemoveAllListeners();
        }

        // Inherited via Updatable
        virtual void OnUpdate() override {
            std::swap(addQueue, sendQueue);

            for (const auto & signal : *sendQueue) {
                FireSignal(signal);
            }
            sendQueue->clear();            
        }
    };

    /**
     * A template-based signal listener.
     * 
     * Service can derive from this class to become a signal listener of
     * a certain type of signals.
     * 
     * **Example**
     * 
     * ```
     * class MyService
     *   : public BaseService
     *   , private SignalListener<std::string>
     * {
     * public:
     * 
     *   // Constructor
     *   MyService();
     * 
     * private:
     *   // Inherited via SignalListener
     *   virtual bool OnSignal(const std::string & signal) override;         
     * };
     * ```
     * 
     * @ingroup srv_group
     */
    template <typename T>
    class SignalListener : virtual public Service, private ISignalListener<T> {
    public:

        SignalListener() {
            AddStartupHook([this]() { 
                ASTU_SERVICE(SignalService<T>).AddListener(*this);
            });

            AddShutdownHook([this]() {
                ASTU_SERVICE(SignalService<T>).RemoveListener(*this);
            });
        }

    protected:
        // Inherited via ISignalListener
        virtual bool OnSignal(const T & signal) override { return false; };        
    };

    /**
     * A template-based signal listener emitter.
     * 
     * Service can derive from this class to easily emit a signal listener of
     * a certain type of signals.
     * 
     * @tparam T    the type of signal to emit
     * @ingroup srv_group
     */
    template <typename T>
    class SignalEmitter : virtual public Service {
    public:

        SignalEmitter(bool fire = false) : fireSignals(fire) {
            AddStartupHook([this]() { 
                signalService = ASTU_GET_SERVICE(SignalService<T>);
            });

            AddShutdownHook([this]() {
                signalService = nullptr;
            });
        }
        
    protected:

        /**
         * Emits the specified signal.
         * 
         * The fire flag defines whether the signal gets fired or queued.
         * 
         * @param signal    the signal to emit
         */
        void EmitSignal(const T & signal) const {
            if (fireSignals) {
                signalService->FireSignal(signal);
            } else {
            signalService->QueueSignal(signal);
            }
        }

        /**
         * Fires the specified signal.
         * 
         * @param signal    the signal to emit
         */
        void FireSignal(const T & signal) const {
            signalService->FireSignal(signal);
        }

        /**
         * Queues the specified signal.
         * 
         * @param signal    the signal to emit
         */
        void QueueSignal(const T & signal) const {
            signalService->QueueSignal(signal);
        }

    private:
        /** The signal service used to emit signals. */
        std::shared_ptr<SignalService<T>> signalService;
        
        /** Whether to fire signals by default. */
        bool fireSignals;
    };


} // end of namespace