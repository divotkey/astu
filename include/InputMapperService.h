/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service.h"

// C++ Standard Library includes
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace astu {

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

    // struct KeyState {
    //     /** Whether the key is currently pressed or released. */
    //     bool pressed;
    // };

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

        friend class InputMapperService;
    };


    class AxisMapping {
    public:

        /**
         * Constructor.
         * 
         * @param name  the name of this mapping
         * @param key   the key of this mapping
         */
        AxisMapping(const std::string & name, const Key &key);

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
        const Key& GetAxis() const;

    private:
        /** The name of this axis mapping. */
        std::string mappingName;

        /** The key of this mapping. */
        Key axis;

        /** A multiplier on the axis value. */
        float scale;
    };


    class AxisBinding {
    public:

        AxisBinding();

        /**
         * Returns the current value of this axis binding.
         * 
         * @return the current axis value
         */
        float GetValue() const;

    private:
        /** The current value. */
        float value;

        void Update(float newValue);

        friend class InputMapperService;
    };

    class InputMapperService : virtual public Service {
    public:

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

        // /**
        //  * Adds an axis mapping.
        //  * 
        //  * @param axisName the unique name of the action
        //  * @param 
        //  */
        // void AddAxisMapping(const std::string & axisName, const Axis& axis);
        // std::shared_ptr<AxisBinding> GetAxisBinding(const std::string & name);


        std::shared_ptr<AxisBinding> BindAxis(const std::string &axisName);

        /**
         * Processes a key event. 
         * 
         * @param key       of the key event
         * @param pressed   whether the key has been pressed or released
         */
        void ProcessKey(const Key & key, bool pressed);

        void ProcessAxis(const Key & key, float value);

    private:
        /** The action mappings, accessible by key. */
        std::map<Key, std::vector<ActionMapping>> actionMappings;

        /** The axis mappings, accessible by key. */
        std::map<Key, std::vector<AxisMapping>> axisMappings;

        /** The bindings to the actions. */
        std::map<std::string, std::vector<std::shared_ptr<ActionBinding>>> actionBindings;

        void ProcessAxisMappings(const Key & key, bool pressed);
        void ProcessActionMappings(const Key & key, bool pressed);
        void UpdateActionBindings(const std::string & actionName, bool pressed);

        bool HasActionMapping(const std::vector<ActionMapping> mappings, const Key & key);
    };

} // end of namespace