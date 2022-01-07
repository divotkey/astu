/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/UpdateService.h"

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

    /**
     * Keys represent input controlls and are caleld Keys for the sake of shortness.
     * A Key can be an actuall key on the keyboard but also a button on the 
     * gamepad or mouse etc. Keys are used to define input mapping in the
     * context of the InputMappingService.
     * 
     * @ingroup input_group
     */
    class Key {
    public:

        /**
         * Constructor.
         * 
         * @param name  the name of this key
         */
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

    /**
     * Key states are used by the InputMappingService internally.
     */
    class KeyState {
    public:

        /**
         * Constructor.
         */
        KeyState() : pressed(false), value(0) {}

        /** Whether the key is pressed. */
        bool pressed;

        /** The value of an axis. */
        float value;
    };

    /////////////////////////////////////////////////
    /////// ActionMapping
    /////////////////////////////////////////////////

    /**
     * Maps an action to a input control (Key).
     * 
     * @ingroup input_group
     */
    class ActionMapping {
    public:

        /**
         * Constructor.
         * 
         * @param name  the name of this mapping
         * @param key   the input control used for this mapping
         */
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

    /**
     * Binds an action to a delegate.
     * 
     * @ingroup input_group
     */
    class ActionBinding {
    public:

        /**
         * Type alias for delegate functions.
         */
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

    /**
     * Maps an axis to an input control (key.
     * 
     * @ingroup input_group
     */
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

    /**
     * Binds an axis to a delegate.
     * 
     * @ingroup input_group
     */
    class AxisBinding {
    public:

        /**
         * Type alias for delegate functions.
         */
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
     * 
     * @ingroup input_group
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
         * @param axisName  the name of the axis to bind to
         * @param delegate  the delegate function of the action binding
         * @return the action binding
         */
        std::shared_ptr<AxisBinding> BindAxis(
            const std::string &axisName, 
            AxisBinding::Delegate delegate = nullptr);

        /**
         * Removes an axis binding.
         * 
         * @param binding   the binding to remove
         */
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
        using ActionBindings = std::vector<std::shared_ptr<ActionBinding>>;
        using AxisBindings = std::vector<std::shared_ptr<AxisBinding>>;
        using ActionMappings = std::vector<ActionMapping>;
        using AxisMappings = std::vector<AxisMapping>;

        /** The current states of keys. */
        std::map<Key, KeyState> keyStates;

        /** The bindings to the actions. */
        std::map<std::string, ActionBindings> actionBindings;

        /** The bindings to axes. */
        std::map<std::string, AxisBindings> axisBindings;

        /** Associates actions to action mappings. */
        std::map<std::string, ActionMappings> actionToMapping;

        /** Associates actions to action mappings. */
        std::map<std::string, AxisMappings> axisToMapping;

        /**
         * Determins whether an action mapping for a certain key exists.
         * 
         * @param mappings  the list of action mappings
         * @param key       the key in question
         * @return `true` if an action mapping for the key exits
         */
        bool HasActionMapping(const ActionMappings &mappings, const Key & key) const;

        /**
         * Determins whether an axis mapping for a certain key exists.
         * 
         * @param mappings  the list of axis mappings
         * @param key       the key in question
         * @return `true` if an axis mapping for the key exits
         */
        bool HasAxisMapping(const AxisMappings &mappings, const Key & key) const;

        /**
         * Returns the state for a given key.
         * 
         * @param key   the key
         * @return the state of the key
         */
        KeyState& GetKeyState(const Key& key);

        /**
         * Ensures that a key state for the given key exits.
         * 
         * @param key   the key
         */
        void EnsureKeyState(const Key& key);

        /**
         * Releases the state of a given key, if the state is not needed anymore.
         * 
         * @param key
         */
        void ReleaseKeyState(const Key& key);

        /**
         * Update axis bindings with specified value.
         * 
         * @param bindings  the action bindings to update
         * @param value     the axus value
         */
        void UpdateAxisBindings(AxisBindings& bindings, float value);

        /**
         * Calculates the value sum of all axis mappings.
         * 
         * @return the value sum
         */
        float SumAxisValue(const std::vector<AxisMapping>& mappings);

        /**
         * Updates all axis binding according to key current states.
         */
        void UpdateAxisBinding();

        /**
         * Updates all action binding according to key current states.
         */
        void UpdateActionBinding();

        /**
         * Find out, if at least one key state of specified actions is pressed.
         * 
         * @param mappings  the action mappings
         * @return `true` if at leas on key state is pressed
         */
        bool IsPressed(const std::vector<ActionMapping>& mappings);

        /**
         * Update action bindings with specified pressed state.
         * 
         * @param bindings  the action bindings to update
         * @param pressed   the pressed state
         */
        void UpdateAcitonBindings(ActionBindings& bindings, bool pressed);

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via Updatable
        virtual void OnUpdate() override;
    };

} // end of namespace