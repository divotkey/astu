/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <string>
#include <algorithm>
#include <limits>
#include "BoundingBox.h"

namespace astu {

    BoundingBox BoundingBox::CreateInfinite()
    {
        BoundingBox result = BoundingBox(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
        result.infinite = true;
        return result;
    }

    BoundingBox::BoundingBox()
    {
        Reset();
    }

    BoundingBox::BoundingBox(double w, double h)
        : infinite(false)
    {
        SetWidth(w);
        SetHeight(h);
    }

    void BoundingBox::Reset()
    {
        center.SetZero();
        SetWidth(0);
        SetHeight(0);
        infinite = false;
    }

    // BoundingBox::BoundingBox(const Vector2<double> & c, double hr, double vr)
    //     : infinite(false)
    // {
    //     if (hr < 0) {
    //         throw std::logic_error("Horizontal radius of bounding box must not be negative, got " 
    //             + std::to_string(hr));
    //     }

    //     if (vr < 0) {
    //         throw std::logic_error("Vertical radius of bounding box must not be negative, got " 
    //             + std::to_string(hr));
    //     }

    //     center = c;
    //     hRadius = hr;
    //     vRadius = vr;
    // }

    void BoundingBox::SetWidth(double w)
    {
        if (w < 0) {
            throw std::domain_error("Width of bounding box must not be negataive, got" 
                + std::to_string(w));
        }
        width = w;
        hRadius = w / 2;
    }

    void BoundingBox::SetHeight(double h)
    {
        if (h < 0) {
            throw std::domain_error("Height of bounding box must not be negataive, got" 
                + std::to_string(h));
        }
        height = h;
        vRadius = h / 2;
    }

    void BoundingBox::SetCenter(const Vector2<double> & c)
    {
        center = c;
    }

    void BoundingBox::MoveCenter(double dx, double dy)
    {
        center.x += dx;
        center.y += dy;
    }

    void BoundingBox::MoveCenter(const Vector2<double> & tx)
    {
        center += tx;        
    }

    void BoundingBox::AddPoint(const Vector2<double> & p)
    {
        if (IsInfinite()) {
            return;
        }

        double left = std::min(GetLeftBound(), p.x);
        double right = std::max(GetRightBound(), p.x);
        double top = std::max(GetUpperBound(), p.y);
        double bottom = std::min(GetLowerBound(), p.y);

        SetWidth(right - left);
        SetHeight(top - bottom);
        // hRadius = (right - left) * 0.5;
        // vRadius = (top - bottom) * 0.5;
        // width = hRadius * 2;
        // height = vRadius * 2;
        center.Set(left + hRadius, bottom + vRadius);
    }

    void BoundingBox::Transform(const Matrix3 & tx)
    {
        if (IsInfinite()) {
            return;
        }

		Vector2<double> p1 = tx.TransformPoint(GetUpperLeft());
		Vector2<double> p2 = tx.TransformPoint(GetUpperRight());
		Vector2<double> p3 = tx.TransformPoint(GetLowerLeft());
		Vector2<double> p4 = tx.TransformPoint(GetLowerRight());

        center = tx.TransformPoint(center);
        hRadius = vRadius = 0;
        AddPoint(p1);
        AddPoint(p2);
        AddPoint(p3);
        AddPoint(p4);
    }

    void BoundingBox::Merge(const BoundingBox & o)
    {
        if (IsInfinite() || o.IsInfinite()) {
            return;
        }

        AddPoint(o.GetUpperLeft());
        AddPoint(o.GetUpperRight());
        AddPoint(o.GetLowerLeft());
        AddPoint(o.GetLowerRight());
    }

    bool BoundingBox::IsInside(const Vector2<double> & p) const
    {
		if (p.x > GetRightBound() || p.x < GetLeftBound()) {
			return false;
		}

		if (p.y > GetUpperBound() || p.y < GetLowerBound()) {
			return false;
		}
		
		return true;
    }

    bool BoundingBox::IsInside(const BoundingBox & box) const
    {
        if (std::abs(center.x - box.center.x) > (hRadius + box.hRadius)) {
            return false;
        }

        if (std::abs(center.y - box.center.y) > (vRadius + box.vRadius)) {
            return false;
        }

        return true;
    }

}