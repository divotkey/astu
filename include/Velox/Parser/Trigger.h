// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// C++ Standard Library includes
#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>

template<typename T>
class Trigger {
public:

    /**
     * Virtual destructor.
     */
    virtual ~Trigger() {}

    bool Evaluate(const T& value) const {
        return (*this)(value);
    }

    virtual bool operator()(const T& value) const = 0;
};

template<typename T>
class AllTrigger : public Trigger<T> {
public:

    AllTrigger() {}

    // Inherited via Trigger<T>
    bool operator()(const T& inValue) const override {
        return true;
    }
};

template<typename T>
class AllTriggerBuilder {
public:

    AllTriggerBuilder() {}

    std::shared_ptr<AllTrigger<T>> Build() {
        return std::make_shared<AllTrigger<T>>();
    }
};

template<typename T>
class EqualTrigger : public Trigger<T> {
public:

    EqualTrigger(T value) : value(value) {}

    // Inherited via Trigger<T>
    bool operator()(const T& inValue) const override {
        return value == inValue;
    }

private:
    T value;
};

template<typename T>
class EqualTriggerBuilder {
public:

    EqualTriggerBuilder() {}

    EqualTriggerBuilder(const T& defaultValue)
        : value(defaultValue) {}

    EqualTriggerBuilder& Value(const T &v) {
        value = v;
        return *this;
    }

    std::shared_ptr<EqualTrigger<T>> Build() {
        return std::make_shared<EqualTrigger<T>>(value);
    }

    std::shared_ptr<EqualTrigger<T>> Build(const T &v) {
        return Value(v).Build();
    }

private:
    T value;
};

template<typename T>
class NotTrigger : public Trigger<T> {
public:

    NotTrigger(std::shared_ptr<Trigger<T>> t) : trigger(t) {}

    // Inherited via Trigger<T>
    bool operator()(const T& inValue) const override {
        return !(*trigger)(inValue);
    }

private:
    std::shared_ptr<Trigger<T>> trigger;
};

template<typename T>
class NotTriggerBuilder {
public:

    NotTriggerBuilder() {}
    NotTriggerBuilder(std::shared_ptr<Trigger<T>> t) : trigger(t) {}

    NotTriggerBuilder& Value(std::shared_ptr<Trigger<T>> t) {
        trigger = t;
        return *this;
    }

    NotTriggerBuilder& Reset() {
        trigger.reset();
        return *this;
    }

    std::shared_ptr<NotTrigger<T>> Build() {
        return std::make_shared<NotTrigger<T>>(trigger);
    }

    std::shared_ptr<EqualTrigger<T>> Build(std::shared_ptr<Trigger<T>> t) {
        return Value(t).Build();
    }

private:
    std::shared_ptr<Trigger<T>> trigger;
};

template<typename T>
class OrTrigger : public Trigger<T> {
public:

    OrTrigger() {}

    OrTrigger(const std::vector<std::shared_ptr<Trigger<T>>> &triggers) : triggers(triggers) {}

    void AddTrigger(std::shared_ptr<Trigger<T>> t) {
        triggers.push_back(t);
    }

    // Inherited via Trigger<T>
    bool operator()(const T& inValue) const override {
        for (const auto &t : triggers) {
            if ((*t)(inValue)) {
                return true;
            }
        }

        return false;
    }

private:
    std::vector<std::shared_ptr<Trigger<T>>> triggers;
};

template<typename T>
class OrTriggerBuilder {
public:

    OrTriggerBuilder& Add(std::shared_ptr<Trigger<T>> t) {
        triggers.push_back(t);
        return *this;
    }

    OrTriggerBuilder& Reset() {
        triggers.clear();
        return *this;
    }

    std::shared_ptr<OrTrigger<T>> Build() {
        return std::make_shared<OrTrigger<T>>(triggers);
    }

private:
    std::vector<std::shared_ptr<Trigger<T>>> triggers;
};

template<typename T>
class RangeTrigger : public Trigger<T> {
public:

    RangeTrigger(const T &minValue, const T &maxValue)
        : minValue(minValue), maxValue(maxValue)
    {
        assert(minValue <= maxValue);
    }

    // Inherited via Trigger<T>
    bool operator()(const T& inValue) const override {
        return inValue >= minValue && inValue <= maxValue;
    }

private:
    T minValue;
    T maxValue;
};

template<typename T>
class RangeTriggerBuilder {
public:

    RangeTriggerBuilder() {}
    RangeTriggerBuilder(const T &minValue, const T &maxValue)
        : minValue(std::min(minValue, maxValue)), maxValue(std::max(minValue, maxValue))
    {}

    RangeTriggerBuilder& Minimum(const T &value) {
        minValue = value;
        return *this;
    }

    RangeTriggerBuilder& Maximum(const T &value) {
        maxValue = value;
        return *this;
    }

    std::shared_ptr<RangeTrigger<T>> Build() {
        return std::make_shared<RangeTrigger<T>>(std::min(minValue, maxValue), std::max(maxValue, minValue));
    }

private:
    T minValue;
    T maxValue;
};