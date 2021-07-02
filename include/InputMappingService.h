/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service.h"

// AST Utilities includes
#include "Service.h"
#include "UpdateService.h"

// C++ Standard Library includes
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace astu {

    /////////////////////////////////////////////////
    /////// Key
    /////////////////////////////////////////////////

    class Key {
    public:

        Key(const std::string & name = "UNKNOWN");

        /**
         * Return the name of this key.
         * 
         * @return the name
         */
        const std::string & GetName() const;

        /**
         * Less than operator on keys.
         * 
         * @param rhs   the right hand side
         */
        const bool operator < ( const Key &rhs ) const {
            return ( name < rhs.name );
        }

        /**
         * Binary equality operator comparing two vectors.
         * 
         * @param rhs   the right hand side
         */
		bool operator==(const Key &rhs) const {
			return name == rhs.name;
		}       

    private:
        std::string name;
    };

    /////////////////////////////////////////////////
    /////// KeyState
    /////////////////////////////////////////////////

    class KeyState {
    public:
        KeyState() : pressed(false), value(0) {}

        bool pressed;
        float value;
    };

    /////////////////////////////////////////////////
    /////// ActionMapping
    /////////////////////////////////////////////////

    class ActionMapping {
    public:

        ActionMapping(const std::string & name, const Key & key);

        /**
         * Returns the name of the action of this mapping.
         * 
         * @return the action
         */
        const std::string & GetName() const;

        /**
         * Return the associated key of this mapping.
         * 
         * @return the key
         */
        const Key & GetKey() const;

    private:
        /** The name of this action mapping. */
        std::string actionName;

        /** The key of this mapping. */
        Key actionKey;
    };

    /////////////////////////////////////////////////
    /////// ActionBinding
    /////////////////////////////////////////////////

    class ActionBinding {
    public:

        using Delegate = std::function<void (ActionBinding &)>;

        /**
         * Constructor.
         * 
         * @param name  the name action of this binding
         */
        ActionBinding(const std::string & name);

        /**
         * Return the name of the action this binding binds to.
         * 
         * @return the action name
         */
        const std::string& GetAction() const;

        /**
         * Returns the current state of this action binding.
         * 
         * @return the current state
         */
        bool IsPressed() const;

        /**
         * Sets the delegate function to be called on state changes.
         * 
         * @param delegate  the the delegate function
         */
        void SetDelegate(Delegate delegate);

    private:
        /** The name of the action. */
        std::string actionName;

        /** The current state of the action. */
        bool pressed;

        /** The delegate function, called on state changes. */
        Delegate delegateFunc;

        /**
         * Updates the state of this action binding.
         * 
         * @param newPressed   the new state
         */
        void Update(bool newPressed);

        friend class InputMappingService;
    };

    /////////////////////////////////////////////////
    /////// AxisMapping
    /////////////////////////////////////////////////

    class AxisMapping {
    public:

        /**
         * Constructor.
         * 
         * @param name  the name of this mapping
         * @param key   the key of this mapping
         * @param scale the multiplier on the axis value
         */
        AxisMapping(const std::string & name, const Key &key, float scale = 1);

        /**
         * Returns the name of the axis of this mapping.
         * 
         * @return the axis name
         */
        const std::string & GetName() const;

        /**
         * Return the associated Key of this mapping.
         * 
         * @return the key
         */
        const Key& GetKey() const;

        /**
         * Returns the axis multiplier.
         * 
         * @return multiplier on the axis value
         */
        float GetScale() const;

    private:
        /** The name of this axis. */
        std::string axisName;

        /** The key of this mapping. */
        Key key;

        /** A multiplier on the axis value. */
        float scale;
    };

    /////////////////////////////////////////////////
    /////// AxisBinding
    /////////////////////////////////////////////////

    class AxisBinding {
    public:

        using Delegate = std::function<void (AxisBinding &)>;

        /**
         * Constructor.
         * 
         * @param axisName  the name of the axis to bind
         */
        AxisBinding(const std::string & axisName);

        /**
         * Returns the current value of this axis binding.
         * 
         * @return the current axis value
         */
        float GetValue() const;

        /**
         * Returns the name of the axis of this binding.
         * 
         * @return the axis name
         */
        const std::string& GetAxis() const;

        /**
         * Sets the delegate function to be called on state changes.
         * 
         * @param delegate  the the delegate function
         */
        void SetDelegate(Delegate delegate);

    private:
        /** The current value. */
        float value;

        /** The name of the axis of this binding. */
        std::string axisName;

        /** The delegate function, called on state changes. */
        Delegate delegateFunc;

        /**
         * Updates the state of this axis binding.
         * 
         * @param newValue  the new axis value of this binding
         */
        void Update(float newValue);

        friend class InputMappingService;
    };

    /////////////////////////////////////////////////
    /////// InputMappingService
    /////////////////////////////////////////////////

    /**
     * This service mapps input events to game actions or axis.
     */
    class InputMappingService 
        : virtual public Service
        , private Updatable
    {
    public:

        /**
         * Constructor.
         * 
         * @param updatePriority the update priority of this
         */
        InputMappingService(Priority updatePriority = Priority::Normal);

        /**
         * Adds mapping for an acton to an input key.
         * 
         * @param actionName    the name of the action
         * @param key           the input key
         */ 
        void AddActionMapping(const std::string & actionName, const Key& key) {
            AddActionMapping( ActionMapping(actionName, key) );
        }

        /**
         * Creates a new action binding.
         * 
         * @param mapping   the action mapping to add
         */ 
        void AddActionMapping(const ActionMapping &mapping);

        /**
         * Creates a new action binding.
         * 
         * @param actionName    the name of the action to bind to
         * @param delegate      the delegate function of the action binding
         * @return the action binding
         */
        std::shared_ptr<ActionBinding> BindAction (
            const std::string & actionName, 
            ActionBinding::Delegate delegate = nullptr
        );

        /**
         * Removes an action binding.
         * 
         * @param binding   the binding to remove
         */
        void RemoveActionBinding(std::shared_ptr<ActionBinding> binding);

        /**
         * Adds an axis mapping.
         * 
         * @param axisName  the unique name of the action
         * @param key       the key of this axis binding
         * @param scale     the multiplier on the axis value
         */
        void AddAxisMapping(
            const std::string & axisName, 
            const Key& key, 
            float scale = 1.0f
        )
        {
            AddAxisMapping( AxisMapping(axisName, key, scale));
        }

        /**
         * Adds an axis mapping.
         * 
         * @param mapping   the axis mapping
         */
        void AddAxisMapping(const AxisMapping & mapping);

        /**
         * Creates a new axis binding.
         * 
         * @param actionName    the name of the axis to bind to
         * @param delegate      the delegate function of the action binding
         * @return the action binding
         */
        std::shared_ptr<AxisBinding> BindAxis(
            const std::string &axisName, 
            AxisBinding::Delegate delegate = nullptr);

        void RemoveAxisBinding(std::shared_ptr<AxisBinding> binding);

        /**
         * Processes a key event. 
         * 
         * @param key       of the key event
         * @param pressed   whether the key has been pressed or released
         */
        void ProcessKey(const Key & key, bool pressed);

        /**
         * Processes a axis input event.
         * 
         * @param key
         * @param value
         */
        void ProcessAxis(const Key & key, float value);

    private:
        /** The current states of keys. */
        std::map<Key, KeyState> keyStates;

        /** The bindings to the actions. */
        std::map<std::string, std::vector<std::shared_ptr<ActionBinding>>> actionBindings;

        /** The bindings to axes. */
        std::map<std::string, std::vector<std::shared_ptr<AxisBinding>>> axisBindings;

        /** Associates actions to action mappings. */
        std::map<std::string, std::vector<ActionMapping>> actionToMapping;

        /** Associates actions to action mappings. */
        std::map<std::string, std::vector<AxisMapping>> axisToMapping;

        bool HasActionMapping(const std::vector<ActionMapping>& mappings, const Key & key) const;
        bool HasAxisMapping(const std::vector<AxisMapping>& mappings, const Key & key) const;
        void EnsureKeyState(const Key& key);
        KeyState& GetKeyState(const Key& key);
        void ReleaseKeyState(const Key& key);

        // Inherited via Updatable
        virtual void OnUpdate() override;
    };

} // end of namespace