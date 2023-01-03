/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "UpdateService.h"
#include "SignalService.h"
#include "Util/ListenerManager.h"

// C++ Standard Library includes
#include <array>
#include <vector>
#include <utility>
#include <mutex>

namespace astu {

    /**
     * A template-based service which is used to transmit objects 
     * called "signals" in multi-threaded environments to registered listeners.
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
     * ASTU_CREATE_AND_ADD_SERVICE(SignalServiceTs<std::string>);
     * ```
     * 
     * To transmit signals of type string the following example code can be
     * used:
     * 
     * ```
     * ASTU_SERVICE(SignalServiceTs<std::string>).QueueSignal("This is a signal");
     * ```
     * 
     * @ingroup srv_group
     */
    template <typename T>
    class SignalServiceTs final : virtual public Service, private Updatable
    {
    public:

        /**
         * Constructor.
         * 
         * @param name      the name of this service
         * @param priority  the update priority of this service
         */
        SignalServiceTs(const std::string & name = "Signal Service", int priority = Normal)
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
        void QueueSignalTs(const T & signal) {
            std::lock_guard<std::mutex> lock(queueMutex);
            addQueue->push_back(signal);
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

        /** Used to synchronize access to signal queue. */
        std::mutex queueMutex;

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

        // Inherited via Service
        virtual void OnShutdown() override {
            std::lock_guard<std::mutex> lock(queueMutex);

            // ClearVariables pending signals and try to free memory used by queues.
            for (auto & queue : signalQueues) {
                queue.clear();
            }

            listenerManager.RemoveAllListeners();
            rawListenerManager.RemoveAllListeners();
        }

        // Inherited via Updatable
        virtual void OnUpdate() override {
            std::lock_guard<std::mutex> lock(queueMutex);

            std::swap(addQueue, sendQueue);

            for (const auto & signal : *sendQueue) {
                FireSignal(signal);
            }
            sendQueue->clear();            
        }
    };

    /**
     * A template-based signal listener for multi-threaded environments.
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
    class SignalListenerTs : virtual public Service, private ISignalListener<T> {
    public:

        /**
         * Constructor.
         */
        SignalListenerTs() {
            AddStartupHook([this]() { 
                ASTU_SERVICE(SignalServiceTs<T>).AddListener(*this);
            });

            AddShutdownHook([this]() {
                ASTU_SERVICE(SignalServiceTs<T>).RemoveListener(*this);
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
    class SignalEmitterTs : virtual public Service {
    public:

        /**
         * Constructor.
         */
        SignalEmitterTs() {
            AddStartupHook([this]() { 
                signalService = ASTU_GET_SERVICE(SignalServiceTs<T>);
            });

            AddShutdownHook([this]() {
                signalService = nullptr;
            });
        }
        
    protected:

        /**
         * Queues the specified signal.
         * 
         * @param signal    the signal to queue
         */
        void QueueSignalTs(const T & signal) const {
            signalService->QueueSignalTs(signal);
        }

    private:
        /** The signal service used to emit signals. */
        std::shared_ptr<SignalServiceTs<T>> signalService;
    };


} // end of namespace