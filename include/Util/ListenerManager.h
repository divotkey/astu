/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <memory>
#include "CommandQueue.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// ListenerManager
    /////////////////////////////////////////////////

    template<typename T>
    class ListenerManager {
    public:

        /**
         * Constructor.
         */
        ListenerManager() : firing(false) {
            // Intentionally left empty.            
        }

        /**
         * Virtual destructor.
         */
        virtual ~ListenerManager() {}

        /**
         * Adds a listener to this manager.
         * 
         * @param listener  the listener to add
         */
        void AddListener(const std::shared_ptr<T> & listener) {
            if (!listener) {
                throw std::logic_error("Listener must not be null");
            }
            
            if (firing) {
                commandQueue.Add([this, listener](){ 
                    AddListenerInternal(listener); 
                });
            } else {
                AddListenerInternal(listener);
            }
        }

        /**
         * Removes a listener from this manager.
         * 
         * @param listener  the listener to remove
         */
        void RemoveListener(const std::shared_ptr<T> & listener) {
            if (firing) {
                commandQueue.Add([this, listener](){ 
                    RemoveListenerInternal(listener); 
                });

                auto it = std::find(listeners.begin(), listeners.end(), listener);
                if (it != listeners.end()) {
                    it->removed = true;
                }

            } else {
                RemoveListenerInternal(listener);
            }
        }

        /**
         * Tests whether a listener has already been added.
         * 
         * @param listener  the listener to test
         */
        bool HasListener(const std::shared_ptr<T> & listener) const {
            auto it = std::find(listeners.begin(), listeners.end(), listener);
            return it != listeners.end() && !it->removed;
        }

        /**
         * Removes all listeners.
         */
        void RemoveAllListeners() {
            listeners.clear();
        }

        /**
         * Calls a given function for all listeners.
         * 
         * The function must take a reference to a listener
         * as parameter.
         * 
         * @param func  the function to call
         */
        void VisitListeners(std::function<bool (T &)> func) {
            firing = true;
            for (auto & deco : listeners) {
                if (!deco.removed) {
                    if (func(*deco.listener)) {
                        // signal has been consumed.
                        break;
                    }
                }
            }
            firing = false;
            commandQueue.Execute();
        }

        /**
         * Returns the number of registered listeners.
         *
         * @return the current number of listeners
         */
        size_t NumListeners() const {
            return listeners.size();
        }

    private:

        /**
         * Inner class used to decorate the listeneres with 'removed' flag.
         */
        class Decorator {
        public:
            Decorator(const std::shared_ptr<T> & _listener) 
                : listener(_listener), removed(false) {}

			// Required for find and erase/remove idiom
			bool operator==(const std::shared_ptr<T> & rhs) const {
				return listener == rhs;
			}

            /** The listener. */
            std::shared_ptr<T> listener;

            /** Indicates whether this listener has been removed. */
            bool removed;
        };

        /** Indicates whether events are currently fired. */
        bool firing;

        /** The managed listeners. */
        std::vector<Decorator> listeners;

        /** Pending pendingCommands. */
        CommandQueue commandQueue;

        void AddListenerInternal(const std::shared_ptr<T> & listener) {
            if (HasListener(listener)) {
                throw std::logic_error("Listener already added");                
            }

            listeners.push_back(Decorator(listener));
        }

        void RemoveListenerInternal(const std::shared_ptr<T> & listener) {
            listeners.erase(
                std::remove(listeners.begin(), listeners.end(), listener), 
                listeners.end()
            );
        }
    };

    /////////////////////////////////////////////////
    /////// SortingListenerManager
    /////////////////////////////////////////////////

    template<typename T>
    class SortingListenerManager {
    public:

        /**
         * Constructor.
         */
        SortingListenerManager() : firing(false) {
            // Intentionally left empty.            
        }

        /**
         * Virtual destructor.
         */
        virtual ~SortingListenerManager() {}

        /**
         * Adds a listener to this manager.
         * 
         * @param listener  the listener to add
         * @param priority  the priority of the listener
         */
        void AddListener(const std::shared_ptr<T> & listener, int priority) {
            if (!listener) {
                throw std::logic_error("Listener must not be null");
            }
            
            if (firing) {
                commandQueue.Add([this, listener, priority](){ 
                    AddListenerInternal(listener, priority); 
                });
            } else {
                AddListenerInternal(listener, priority);
            }
        }

        /**
         * Removes a listener from this manager.
         * 
         * @param listener  the listener to remove
         */
        void RemoveListener(const std::shared_ptr<T> & listener) {
            if (firing) {
                commandQueue.Add([this, listener](){ 
                    RemoveListenerInternal(listener); 
                });

                auto it = std::find(listeners.begin(), listeners.end(), listener);
                if (it != listeners.end()) {
                    it->removed = true;
                }

            } else {
                RemoveListenerInternal(listener);
            }
        }

        /**
         * Tests whether a listener has already been added.
         * 
         * @param listener  the listener to test
         */
        bool HasListener(const std::shared_ptr<T> & listener) const {
            auto it = std::find(listeners.begin(), listeners.end(), listener);
            return it != listeners.end() && !it->removed;
        }

        /**
         * Removes all listeners.
         */
        void RemoveAllListeners() {
            listeners.clear();
        }

        /**
         * Returns the number of registered listeners.
         *
         * @return the current number of listeners
         */
        size_t NumListeners() const {
            return listeners.size();
        }

        /**
         * Calls a given function for all listeners.
         * 
         * The function must take a reference to a listener
         * as parameter.
         * 
         * @param func  the function to call
         */
        void VisitListeners(std::function<bool (T &)> func) {
            firing = true;
            for (auto & deco : listeners) {
                if (!deco.removed) {
                    if (func(*deco.listener)) {
                        // signal has been consumed.
                        break;
                    }
                }
            }
            firing = false;
            commandQueue.Execute();
        }

    private:

        /**
         * Inner class used to decorate the listeneres with 'removed' flag.
         */
        class Decorator {
        public:
            Decorator(const std::shared_ptr<T> & _listener, int p) 
                : listener(_listener), priority(p), removed(false) {}

			// Required for find and erase/remove idiom
			bool operator==(const std::shared_ptr<T> & rhs) const {
				return listener == rhs;
			}

			// Required for sorting
			bool operator<(const Decorator & rhs) const {
				return priority < rhs.priority;
			}

            /** The listener. */
            std::shared_ptr<T> listener;

            /** Listeners get sorted according to this priority. */
            int priority;

            /** Indicates whether this listener has been removed. */
            bool removed;
        };

        /** Indicates whether events are currently fired. */
        bool firing;

        /** The managed listeners. */
        std::vector<Decorator> listeners;

        /** Pending pendingCommands. */
        CommandQueue commandQueue;

        void AddListenerInternal(const std::shared_ptr<T> & listener, int priority) {
            if (HasListener(listener)) {
                throw std::logic_error("Listener already added");                
            }

            listeners.push_back(Decorator(listener, priority));
            std::sort(listeners.begin(), listeners.end());
        }

        void RemoveListenerInternal(const std::shared_ptr<T> & listener) {
            listeners.erase(
                std::remove(listeners.begin(), listeners.end(), listener), 
                listeners.end()
            );
        }
    };

    /////////////////////////////////////////////////
    /////// RawListenerManager
    /////////////////////////////////////////////////

    template<typename T>
    class RawListenerManager {
    public:

        /**
         * Constructor.
         */
        RawListenerManager() : firing(false) {
            // Intentionally left empty.            
        }

        /**
         * Virtual destructor.
         */
        virtual ~RawListenerManager() {}

        /**
         * Adds a listener to this manager.
         * 
         * @param pListener  the listener to add
         */
        void AddListener(T* pListener) {
            if (!pListener) {
                throw std::logic_error("Listener must not be null");
            }
            
            if (firing) {
                commandQueue.Add([this, pListener](){ 
                    AddListenerInternal(pListener); 
                });
            } else {
                AddListenerInternal(pListener);
            }
        }

        /**
         * Removes a listener from this manager.
         * 
         * @param listener  the listener to remove
         */
        void RemoveListener(T* pListener) {
            if (firing) {
                commandQueue.Add([this, pListener](){ 
                    RemoveListenerInternal(pListener); 
                });

                auto it = std::find(listeners.begin(), listeners.end(), pListener);
                if (it != listeners.end()) {
                    it->removed = true;
                }

            } else {
                RemoveListenerInternal(pListener);
            }
        }

        /**
         * Tests whether a listener has already been added.
         * 
         * @param listener  the listener to test
         */
        bool HasListener(const T* pListener) const {
            auto it = std::find(listeners.begin(), listeners.end(), pListener);
            return it != listeners.end() && !it->removed;
        }

        /**
         * Removes all listeners.
         */
        void RemoveAllListeners() {
            listeners.clear();
        }

        /**
         * Returns the number of registered listeners.
         *
         * @return the current number of listeners
         */
        size_t NumListeners() const {
            return listeners.size();
        }

        /**
         * Calls a given function for all listeners.
         * 
         * The function must take a reference to a listener
         * as parameter.
         * 
         * @param func  the function to call
         */
        void VisitListeners(std::function<bool (T &)> func) {
            firing = true;
            for (auto & deco : listeners) {
                if (!deco.removed) {
                    if (func(*deco.listener)) {
                        // signal has been consumed.
                        break;
                    }
                }
            }
            firing = false;
            commandQueue.Execute();
        }

    private:

        /**
         * Inner class used to decorate the listeners with 'removed' flag.
         */
        class Decorator {
        public:
            Decorator(T * pListener) 
                : listener(pListener), removed(false) {}

			// Required for find and erase/remove idiom
			bool operator==(const T* rhs) const {
				return listener == rhs;
			}

            /** The listener. */
            T * listener;

            /** Indicates whether this listener has been removed. */
            bool removed;
        };

        /** Indicates whether events are currently fired. */
        bool firing;

        /** The managed listeners. */
        std::vector<Decorator> listeners;

        /** Pending pendingCommands. */
        CommandQueue commandQueue;


        void AddListenerInternal(T * pListener) {
            if (HasListener(pListener)) {
                throw std::logic_error("Listener already added");                
            }

            listeners.push_back(Decorator(pListener));
        }

        void RemoveListenerInternal(T * pListener) {
            listeners.erase(
                std::remove(listeners.begin(), listeners.end(), pListener), 
                listeners.end()
            );
        }
    };

    /////////////////////////////////////////////////
    /////// SortingRawListenerManager
    /////////////////////////////////////////////////

    template<typename T>
    class SortingRawListenerManager {
    public:

        /**
         * Constructor.
         */
        SortingRawListenerManager() : firing(false) {
            // Intentionally left empty.            
        }

        /**
         * Virtual destructor.
         */
        virtual ~SortingRawListenerManager() {}

        /**
         * Adds a listener to this manager.
         * 
         * @param listener  the listener to add
         * @param priority  the priority of the listener
         */
        void AddListener(T* listener, int priority) {
            if (!listener) {
                throw std::logic_error("Listener must not be null");
            }
            
            if (firing) {
                commandQueue.Add([this, listener, priority](){ 
                    AddListenerInternal(listener, priority); 
                });
            } else {
                AddListenerInternal(listener, priority);
            }
        }

        /**
         * Removes a listener from this manager.
         * 
         * @param listener  the listener to remove
         */
        void RemoveListener(T* listener) {
            if (firing) {
                commandQueue.Add([this, listener](){ 
                    RemoveListenerInternal(listener); 
                });

                auto it = std::find(listeners.begin(), listeners.end(), listener);
                if (it != listeners.end()) {
                    it->removed = true;
                }

            } else {
                RemoveListenerInternal(listener);
            }
        }

        /**
         * Tests whether a listener has already been added.
         * 
         * @param listener  the listener to test
         */
        bool HasListener(T* listener) const {
            auto it = std::find(listeners.begin(), listeners.end(), listener);
            return it != listeners.end() && !it->removed;
        }

        /**
         * Removes all listeners.
         */
        void RemoveAllListeners() {
            listeners.clear();
        }

        /**
         * Returns the priority of the specified listener.
         * 
         * @param listener  the listener
         * @return the listener's priority
         */
        int GetListenerPriority(T* listener) const {
            auto it = std::find(listeners.begin(), listeners.end(), listener);
            if (it == listeners.end()) {
                throw std::logic_error("Unknown listener");
            }

            return it->priority;
        }

        /**
         * Returns the number of registered listeners.
         *
         * @return the current number of listeners
         */
        size_t NumListeners() const {
            return listeners.size();
        }

        /**
         * Calls a given function for all listeners.
         * 
         * The function must take a reference to a listener
         * as parameter.
         * 
         * @param func  the function to call
         */
        void VisitListeners(std::function<bool (T &)> func) {
            firing = true;
            for (auto & deco : listeners) {
                if (!deco.removed) {
                    if (func(*deco.listener)) {
                        // signal has been consumed.
                        break;
                    }
                }
            }
            firing = false;
            commandQueue.Execute();
        }

    private:

        /**
         * Inner class used to decorate the listeners with 'removed' flag.
         */
        class Decorator {
        public:
            Decorator(T* listener, int p) 
                : listener(listener), priority(p), removed(false) {}

			// Required for find and erase/remove idiom
			bool operator==(const T* rhs) const {
				return listener == rhs;
			}

			// Required for sorting
			bool operator<(const Decorator & rhs) const {
				return priority < rhs.priority;
			}

            /** The listener. */
            T* listener;

            /** Listeners get sorted according to this priority. */
            int priority;

            /** Indicates whether this listener has been removed. */
            bool removed;
        };

        /** Indicates whether events are currently fired. */
        bool firing;

        /** The managed listeners. */
        std::vector<Decorator> listeners;

        /** Pending pendingCommands. */
        CommandQueue commandQueue;


        void AddListenerInternal(T* listener, int priority) {
            if (HasListener(listener)) {
                throw std::logic_error("Listener already added");                
            }

            listeners.push_back(Decorator(listener, priority));
            std::sort(listeners.begin(), listeners.end());
        }

        void RemoveListenerInternal(T* listener) {
            listeners.erase(
                std::remove(listeners.begin(), listeners.end(), listener), 
                listeners.end()
            );
        }
    };

} // end of namespace