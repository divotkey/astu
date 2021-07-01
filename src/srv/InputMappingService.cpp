/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "InputMappingService.h"

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
    /////// AxisMapping
    /////////////////////////////////////////////////

    AxisMapping::AxisMapping(const std::string & name, const Key &key, float scale)
        : axisName(name)
        , key(key)
        , scale(scale)
    {
        // Intentionally left empty.
    }

    const std::string & AxisMapping::GetName() const
    {
        return axisName;
    }

    const Key& AxisMapping::GetKey() const
    {
        return key;
    }

    float AxisMapping::GetScale() const
    {
        return scale;
    }

    /////////////////////////////////////////////////
    /////// AxisBinding
    /////////////////////////////////////////////////

    AxisBinding::AxisBinding(const std::string & axisName)
        : axisName(axisName)
        , value(0)
        , prevValue(0)
    {
        // Intentionally left empty.
    }

    float AxisBinding::GetValue() const
    {
        return value;
    }


    const std::string& AxisBinding::GetAxis() const
    {
        return axisName;
    }

    void AxisBinding::SetDelegate(Delegate delegate) {
        delegateFunc = delegate;
    }

    void AxisBinding::Update()
    {
        if (value != prevValue) {
            if (delegateFunc) {
                delegateFunc(*this);
            }
            prevValue = value;
        }
    }

    /////////////////////////////////////////////////
    /////// InputMapperService
    /////////////////////////////////////////////////

    InputMappingService::InputMappingService(Priority updatePriority)
        : Service("Input mapping service")
        , Updatable(updatePriority)
    {
        // Intentionally left empty.
    }

    bool InputMappingService::HasActionMapping(
        const vector<ActionMapping> &mappings, 
        const Key & key) const
    {
        for (const auto & mapping : mappings) {
            if (mapping.GetKey() == key) {
                return true;
            }
        }

        return false;
    }

    bool InputMappingService::HasAxisMapping(const std::vector<AxisMapping>& mappings, const Key & key) const
    {
        for (const auto & mapping : mappings) {
            if (mapping.GetKey() == key) {
                return true;
            }
        }

        return false;
    }

    void InputMappingService::AddActionMapping(const ActionMapping &mapping)
    {
        auto it = actionToMapping.find(mapping.GetName());
        if (it == actionToMapping.end()) {
            actionToMapping[mapping.GetName()].push_back(mapping);
        } else if (!HasActionMapping(it->second, mapping.GetKey())){
            it->second.push_back(mapping);
        }

        EnsureKeyState(mapping.GetKey());
    }

    shared_ptr<ActionBinding> InputMappingService::BindAction (
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

    void InputMappingService::RemoveActionBinding(std::shared_ptr<ActionBinding> binding)
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

    void InputMappingService::AddAxisMapping(const AxisMapping & mapping)
    {
        auto it = axisMappings.find(mapping.GetKey());
        if (it == axisMappings.end()) {
            axisMappings[mapping.GetKey()].push_back(mapping);
            return;
        }

        if (!HasAxisMapping(it->second, mapping.GetKey())) {
            it->second.push_back(mapping);
        }

        EnsureKeyState(mapping.GetKey());
    }

    std::shared_ptr<AxisBinding> InputMappingService::BindAxis(const std::string &axisName)
    {
        auto binding = std::make_shared<AxisBinding>(axisName);
        // binding->SetDelegate(delegate);

        auto it = axisBindings.find(binding->GetAxis());
        if (it == axisBindings.end()) {
            axisBindings[binding->GetAxis()].push_back(binding);
        }

        return binding; 
    }

    void InputMappingService::RemoveAxisBinding(std::shared_ptr<AxisBinding> binding)
    {
        auto it = axisBindings.find(binding->GetAxis());
        if (it == axisBindings.end()) {
            return;
        }

        auto bindings = it->second;
        bindings.erase(remove(bindings.begin(), bindings.end(), binding), bindings.end());        
        if (it->second.empty()) {
            axisBindings.erase(it);
        }
    }

    void InputMappingService::ProcessKey(const Key & key, bool pressed)
    {
        auto it = keyStates.find(key);
        if (it == keyStates.end()) {
            return;
        }

        auto &keyState = it->second;
        keyState.pressed = pressed;
        keyState.value = pressed ? 1.0f : 0.0f;
    }

    // void InputMappingService::ProcessActionMappings(const Key & key, bool pressed)
    // {
    //     auto it = actionMappings.find(key);
    //     if (it == actionMappings.end()) {
    //         return;
    //     }

    //     std::vector<ActionMapping> mappings = it->second;
    //     for (auto & mapping : mappings) {
    //         UpdateActionBindings(mapping.GetName(), pressed);
    //     }
    // }

    // void InputMappingService::UpdateActionBindings(
    //     const std::string & actionName, 
    //     bool pressed)
    // {
    //     auto it = actionBindings.find(actionName);
    //     if (it == actionBindings.end()) {
    //         return;
    //     }

    //     for (auto & binding : it->second) {
    //         binding->Update(pressed);
    //     }
    // }

    // void InputMappingService::ProcessAxisMappings(const Key & key, bool pressed)
    // {
    //     auto it = axisMappings.find(key);
    //     if (it == axisMappings.end()) {
    //         return;
    //     }

    //     auto mappings = it->second;
    //     for (auto & mapping : mappings) {
    //         UpdateAxisBindings(mapping.GetName(), mapping.GetScale());
    //     }
    // }

    // void InputMappingService::UpdateAxisBindings(const std::string & axisName, float value)
    // {
    //     auto it = axisBindings.find(axisName);
    //     if (it == axisBindings.end()) {
    //         return;
    //     }

    //     auto bindings = it->second;
    //     for (const auto& binding : bindings) {
    //         binding->value += value;
    //     }
    // }

    void InputMappingService::OnUpdate()
    {
        // Update action bindings.
        for (const auto& it : actionToMapping) {
            auto bindingIt = actionBindings.find(it.first);
            if (bindingIt == actionBindings.end()) {
                // No action bindings for this action, continue with next.
                continue;
            }

            // Find out, if at least one key is pressed for this action.
            bool pressed;
            for (const auto & mapping : it.second) {
                pressed = GetKeyState(mapping.GetKey()).pressed;
                if (pressed) 
                    break;
            }
            
            // Update all bindings of this action.
            for (const auto & binding : bindingIt->second) {
                binding->Update(pressed);
            }
        }

        // for (const auto& it : actionMappings) {
        //     auto keyState = GetKeyState(it.first);
        //     const auto& mappings = it.second;
        //     for (mapping : )
        //     it.second->
        // }

        // for (const auto& it : actionBindings) {
        //     for (const auto& binding : it.second) {
        //         binding->Update()
        //     }
        // }

        // for (const auto& it : axisBindings) {
        //     for (const auto& binding : it.second) {
        //         binding->Update();
        //         binding->value = 0;
        //     }
        // }
    }

    void InputMappingService::EnsureKeyState(const Key& key)
    {
        auto it = keyStates.find(key);
        if (it == keyStates.end()) {
            keyStates[key] = KeyState();
        }
    }

    KeyState& InputMappingService::GetKeyState(const Key& key)
    {
        assert(keyStates.find(key) != keyStates.end());
        return keyStates[key];
    }

    void InputMappingService::ReleaseKeyState(const Key& key)
    {
        // Not impoemented.
    }

} // end of namespace