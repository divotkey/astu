/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Input/InputMappingService.h"

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

    ActionMapping::ActionMapping(const string & name, const Key & key)
        : actionName(name)
        , actionKey(key)
    {
        // Intentionally left empty.
    }
    
    const string & ActionMapping::GetName() const
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

    ActionBinding::ActionBinding(const string & name)
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

    AxisMapping::AxisMapping(const string & name, const Key &key, float scale)
        : axisName(name)
        , key(key)
        , scale(scale)
    {
        // Intentionally left empty.
    }

    const string & AxisMapping::GetName() const
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

    AxisBinding::AxisBinding(const string & axisName)
        : axisName(axisName)
        , value(0)
    {
        // Intentionally left empty.
    }

    float AxisBinding::GetValue() const
    {
        return value;
    }

    const string& AxisBinding::GetAxis() const
    {
        return axisName;
    }

    void AxisBinding::SetDelegate(Delegate delegate) {
        delegateFunc = delegate;
    }

    void AxisBinding::Update(float newValue)
    {
        if (value != newValue) {
            value = newValue;
            if (delegateFunc) {
                delegateFunc(*this);
            }
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

    void InputMappingService::OnStartup() 
    {
        // Intentionally left empty.
    }

    void InputMappingService::OnShutdown() 
    {
        // Cleanup.
        actionToMapping.clear();
        axisToMapping.clear();
        keyStates.clear();
    }

    bool InputMappingService::HasActionMapping(const ActionMappings &mappings, const Key & key) const
    {
        for (const auto & mapping : mappings) {
            if (mapping.GetKey() == key) {
                return true;
            }
        }

        return false;
    }

    bool InputMappingService::HasAxisMapping(const AxisMappings& mappings, const Key & key) const
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
        const string & actionName, 
        ActionBinding::Delegate delegate)
    {
        auto binding = make_shared<ActionBinding>(actionName);
        binding->SetDelegate(delegate);

        auto it = actionBindings.find(binding->GetAction());
        if (it == actionBindings.end()) {
            actionBindings[binding->GetAction()].push_back(binding);
        } else {
            it->second.push_back(binding);
        }

        return binding; 
    }

    void InputMappingService::RemoveActionBinding(shared_ptr<ActionBinding> binding)
    {
        auto it = actionBindings.find(binding->GetAction());
        if (it == actionBindings.end()) {
            return;
        }

        auto& bindings = it->second;
        bindings.erase(remove(bindings.begin(), bindings.end(), binding), bindings.end());        
        if (it->second.empty()) {
            actionBindings.erase(it);
        }
    }

    void InputMappingService::AddAxisMapping(const AxisMapping & mapping)
    {
        auto it = axisToMapping.find(mapping.GetName());
        if (it == axisToMapping.end()) {
            axisToMapping[mapping.GetName()].push_back(mapping);
        } else if (!HasAxisMapping(it->second, mapping.GetKey())){
            it->second.push_back(mapping);
        }

        EnsureKeyState(mapping.GetKey());
    }

    shared_ptr<AxisBinding> InputMappingService::BindAxis(
        const string &axisName, 
        AxisBinding::Delegate delegate
    )
    {
        auto binding = make_shared<AxisBinding>(axisName);
        binding->SetDelegate(delegate);

        auto it = axisBindings.find(binding->GetAxis());
        if (it == axisBindings.end()) {
            axisBindings[binding->GetAxis()].push_back(binding);
        } else {
            it->second.push_back(binding);
        }

        return binding; 
    }

    void InputMappingService::RemoveAxisBinding(shared_ptr<AxisBinding> binding)
    {
        auto it = axisBindings.find(binding->GetAxis());
        if (it == axisBindings.end()) {
            return;
        }

        auto& bindings = it->second;
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

    void InputMappingService::ProcessAxis(const Key & key, float value)
    {
        auto it = keyStates.find(key);
        if (it == keyStates.end()) {
            return;
        }

        auto &keyState = it->second;
        keyState.pressed = value != 0;
        keyState.value = value;
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
        // Not implemented.
    }

    bool InputMappingService::IsPressed(const ActionMappings &mappings)
    {
        for (const auto & mapping : mappings) {
            if (GetKeyState(mapping.GetKey()).pressed) {
                return true;
            }
        }

        return false;
    }

    void InputMappingService::UpdateAcitonBindings(ActionBindings &bindings, bool pressed)
    {
        for (const auto & binding : bindings) {
            binding->Update(pressed);
        }
    }

    void InputMappingService::UpdateActionBinding()
    {
        for (const auto& it : actionToMapping) {
            auto bindingIt = actionBindings.find(it.first);

            if (bindingIt == actionBindings.end()) {
                // No action bindings for this action, continue with next.
                continue;
            }

            // Update all bindigs with summerzied pressed state.
            UpdateAcitonBindings(bindingIt->second, IsPressed(it.second));
        }
    }

    void InputMappingService::UpdateAxisBindings(AxisBindings& bindings, float value)
    {
        // Update all bindings of this action.
        for (const auto & binding : bindings) {
            binding->Update(value);
        }
    }

    float InputMappingService::SumAxisValue(const AxisMappings &mappings)
    {
        float value = 0.0;
        for (const auto & mapping : mappings) {
            value += GetKeyState(mapping.GetKey()).value * mapping.GetScale();
        }

        return value;
    }

    void InputMappingService::UpdateAxisBinding()
    {
        for (const auto& it :axisToMapping) {
            auto bindingIt = axisBindings.find(it.first);
            if (bindingIt == axisBindings.end()) {
                // No action bindings for this action, continue with next.
                continue;
            }

            // Update axis bindings with sum of values.
            UpdateAxisBindings(bindingIt->second, SumAxisValue(it.second));
        }        
    }

    void InputMappingService::OnUpdate()
    {
        UpdateActionBinding();
        UpdateAxisBinding();
    }

} // end of namespace