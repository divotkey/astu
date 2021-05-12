/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard library includes
#include <cmath>
#include <iostream>

// Local includes
#include "Vector2.h"
#include "Matrix3.h"

template <typename T>
class Transform2 {
public:

    void SetScaling(const Vector2<T> & s) {
        scaling = s;
    }

    const Vector2<T> & GetScaling() const {
        return scaling;
    }

private:
    /** Translation of this transformation, as a vector. */
    astu::Vector2<T> translation;

    /** 2D scale (always applied in local space) as a vector. */
    astu::Vector2<T> scaling;

    /** Rotation of this transformation, in radian. */
    T rotation;

    /** The transformation matrix of this transform. */
    Matrix3<t> matrix;
};