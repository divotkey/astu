/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes.
#include "Service/Tasks.h"

// C++ Standard Library inlcudes
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// TimedTask
    /////////////////////////////////////////////////

    void TimedTask::SetDuration(double inDuration)
    {
        if (inDuration < 0) {
            throw std::domain_error("Duration must be greater zero");
        }
        duration = inDuration;
    }

    /////////////////////////////////////////////////
    /////// DelegateTask
    /////////////////////////////////////////////////

    // Required for pooling.
    // template<> std::vector<DelegateTask *> Pooled<DelegateTask>::ms_rawPool;
    // template<> std::vector<DelegateTask *> Pooled<DelegateTask>::ms_rawUsed;

    unique_ptr<DelegateTask> DelegateTask::Create(Delegate delegate, double delay)
    {
        return unique_ptr<DelegateTask>(new DelegateTask(delegate, delay));
    }

    DelegateTask::DelegateTask(Delegate delegate, double delay)
        : delegateFunc(delegate)
        , delay(delay)
    {
        PREPARE_POOLING();
        Reset();
    }

    DelegateTask::~DelegateTask()
    {
        // Intentionally left empty.
    }

    void DelegateTask::Reset()
    {
        elapsedTime = 0;
    }

    void DelegateTask::Update(double dt)
    {
        elapsedTime += dt;
        if (elapsedTime >= delay) {
            if (delegateFunc) {
                delegateFunc();
            }
            Terminate();
        }
    }

    DelegateTaskBuilder::DelegateTaskBuilder()
    {
        Reset();
    }

    DelegateTaskBuilder& DelegateTaskBuilder::Reset()
    {
        TaskBuilder::Reset();
        delay = 0;
        delegateFunc = nullptr;
        return *this;
    }

    DelegateTaskBuilder& DelegateTaskBuilder::Delegate(DelegateTask::Delegate delegate)
    {
        delegateFunc = delegate;
        return *this;
    }

    DelegateTaskBuilder& DelegateTaskBuilder::Delay(double inDelay)
    {
        delay = inDelay;
        return *this;
    }

    std::unique_ptr<DelegateTask> DelegateTaskBuilder::DelegateTaskBuilder::Build()
    {
        auto result = DelegateTask::Create(delegateFunc, delay);
        TaskBuilder::Build(*result);
        return result;
    }


} // end of namespace