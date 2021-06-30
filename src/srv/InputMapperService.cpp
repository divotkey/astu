/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "InputMapperService.h"

// C++ Standard Library includes
#include <algorithm>

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// Key
    /////////////////////////////////////////////////

    Key::Key(const string & name)
        : name (name)
    {
        // Intentionally left empty.
    }

    const string & Key::GetName() const
    {
        return name;
    }

    /////////////////////////////////////////////////
    /////// ActionMapping
    /////////////////////////////////////////////////

    ActionMapping::ActionMapping(const std::string & name, const Key & key)
        : actionName(name)
        , actionKey(key)
    {
        // Intentionally left empty.
    }
    
    const std::string & ActionMapping::GetName() const
    {
        return actionName;
    }

    const Key & ActionMapping::GetKey() const
    {
        return actionKey;
    }

    /////////////////////////////////////////////////
    /////// ActionBinding
    /////////////////////////////////////////////////

    ActionBinding::ActionBinding(const std::string & name)
        : actionName(name)
        , pressed(false)
    {
        // Intentionally left empty.
    }

    const string& ActionBinding::GetAction() const
    {
        return actionName;
    }

    bool ActionBinding::IsPressed() const
    {
        return pressed;
    }

    void ActionBinding::SetDelegate(Delegate delegate)
    {
        delegateFunc = delegate;
    }
    
    void ActionBinding::Update(bool newPressed)
    {
        if (pressed != newPressed) {
            pressed = newPressed;
            if (delegateFunc) {
                delegateFunc(*this);
            }
        }
    }

    /////////////////////////////////////////////////
    /////// AxisBinding
    /////////////////////////////////////////////////

    AxisBinding::AxisBinding()
        : value(0)
    {
        // Intentionally left empty.
    }

    /////////////////////////////////////////////////
    /////// InputMapperService
    /////////////////////////////////////////////////

    bool InputMapperService::HasActionMapping(
        const vector<ActionMapping> mappings, 
        const Key & key)
    {
        for (const auto & mapping : mappings) {
            if (mapping.GetKey() == key) {
                return true;
            }
        }

        return false;
    }

    void InputMapperService::AddActionMapping(const ActionMapping &mapping)
    {
        auto it = actionMappings.find(mapping.GetKey());
        if (it == actionMappings.end()) {
            actionMappings[mapping.GetKey()].push_back(mapping);
            return;
        }

        if (!HasActionMapping(it->second, mapping.GetKey())) {
            it->second.push_back(mapping);
        }
    }

    shared_ptr<ActionBinding> InputMapperService::BindAction (
        const std::string & actionName, 
        ActionBinding::Delegate delegate)
    {
        auto binding = std::make_shared<ActionBinding>(actionName);
        binding->SetDelegate(delegate);

        auto it = actionBindings.find(binding->GetAction());
        if (it == actionBindings.end()) {
            actionBindings[binding->GetAction()].push_back(binding);
        }

        return binding; 
    }

    void InputMapperService::RemoveActionBinding(std::shared_ptr<ActionBinding> binding)
    {
        auto it = actionBindings.find(binding->GetAction());
        if (it == actionBindings.end()) {
            return;
        }

        auto bindings = it->second;
        bindings.erase(remove(bindings.begin(), bindings.end(), binding), bindings.end());        
        if (it->second.empty()) {
            actionBindings.erase(it);
        }
    }

    void InputMapperService::ProcessKey(const Key & key, bool pressed)
    {
        ProcessActionMappings(key, pressed);
        ProcessAxisMappings(key, pressed);
    }

    void InputMapperService::ProcessActionMappings(const Key & key, bool pressed)
    {
        auto it = actionMappings.find(key);
        if (it == actionMappings.end()) {
            return;
        }

        std::vector<ActionMapping> mappings = it->second;
        for (auto & mapping : mappings) {
            UpdateActionBindings(mapping.GetName(), pressed);
        }
    }

    void InputMapperService::UpdateActionBindings(
        const std::string & actionName, 
        bool pressed)
    {
        auto it = actionBindings.find(actionName);
        if (it == actionBindings.end()) {
            return;
        }

        for (auto & binding : it->second) {
            binding->Update(pressed);
        }
    }

    void InputMapperService::ProcessAxisMappings(const Key & key, bool pressed)
    {
        auto it = axisMappings.find(key);
        if (it == axisMappings.end()) {
            return;
        }

        std::vector<AxisMapping> mappings = it->second;
        for (auto & mapping : mappings) {
            // mapping.
        }
    }



} // end of namespace