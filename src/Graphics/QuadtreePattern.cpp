/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/QuadtreePattern.h"
#include "Graphics/UnicolorPattern.h"
#include "Math/Random.h"

// C++ Standard Library includes
#include <cassert>
#include <iostream>
#include <string>

using namespace std;

namespace astu {

    QuadtreePattern::QuadtreePattern(int inMaxElems, int inMaxDepth, bool debug)
        : depth(0)
        , leaf(false)
        , debug(debug)
        , scan(false)
        , scanFactor(1.0 / 100.0)
    {
        SetMaxElems(inMaxElems);
        SetMaxDepth(inMaxDepth);
    }

    void QuadtreePattern::SetMaxElems(int n)
    {
        if (n <= 0) {
            throw std::domain_error("Maximum number of elements for quadtree must be greater zero, got "
                + to_string(n));
        }

        maxElems = n;
    }

    void QuadtreePattern::SetMaxDepth(int n)
    {
        if (n <= 0) {
            throw std::domain_error("Maximum depth of quadtree mast be greater zero, got "
                + to_string(n));
        }

        maxDepth = n;
    }

    void QuadtreePattern::BuildTree()
    {
        if (static_cast<int>(children.size()) < maxElems || depth >= maxDepth) {
            leaf = true;

            if (debug) {
                debugPattern = make_shared<RectanglePattern>(localBox.GetWidth(), localBox.GetHeight());
                debugPattern->Translate(localBox.GetCenter());
                    debugPattern->SetPattern(UnicolorPattern::CreateRandomColored(0.5));
                    children.push_back(debugPattern);
            } else if (debugPattern) {
                RemovePattern(debugPattern);
                debugPattern = nullptr;
            }
            return;
        }
        leaf = false;

        upperLeft = CreateNode(-localBox.GetHRadius() / 2, localBox.GetVRadius() / 2);
        upperRight = CreateNode(localBox.GetHRadius() / 2, localBox.GetVRadius() / 2);
        lowerLeft = CreateNode(-localBox.GetHRadius() / 2, -localBox.GetVRadius() / 2);
        lowerRight = CreateNode(localBox.GetHRadius() / 2, -localBox.GetVRadius() / 2);

        for (auto child : children) {

            if (scan) {
                if (upperLeft->IsInsideScan(*child)) {
                    upperLeft->AddPattern(child);
                }

                if (upperRight->IsInsideScan(*child)) {
                    upperRight->AddPattern(child);
                }

                if (lowerLeft->IsInsideScan(*child)) {
                    lowerLeft->AddPattern(child);
                }

                if (lowerRight->IsInsideScan(*child)) {
                    lowerRight->AddPattern(child);
                }
            } else {
                const auto & box = child->GetBoundingBox();
                if (upperLeft->localBox.IsInside(box)) {
                    upperLeft->AddPattern(child);
                }

                if (upperRight->localBox.IsInside(box)) {
                    upperRight->AddPattern(child);
                }

                if (lowerLeft->localBox.IsInside(box)) {
                    lowerLeft->AddPattern(child);
                }

                if (lowerRight->localBox.IsInside(box)) {
                    lowerRight->AddPattern(child);
                }
            }


        }

        upperLeft->BuildTree();
        upperRight->BuildTree();
        lowerLeft->BuildTree();
        lowerRight->BuildTree();
        // children.clear();
    }

    void QuadtreePattern::OnPatternAdded(Pattern & pattern)
    {
        if (leaf) {
            return;
        }
        const auto & box = pattern.GetBoundingBox();

        if (localBox.IsEmpty() && !box.IsInfinite()) {
            localBox = box;
        } else {
            localBox.Merge(box);
        }
    }

    void QuadtreePattern::OnClear()
    {
        localBox.Reset();
        upperLeft.reset();
        upperRight.reset();
        lowerLeft.reset();
        lowerRight.reset();
    }

    BoundingBox QuadtreePattern::GetLocalBoundingBox() const
    {
        return localBox;
    }

    bool QuadtreePattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        if (!localBox.IsInside(pt)) {
            return false;
        }

        if (!leaf) {
            if (pt.x > localBox.GetCenter().x) {
                if (pt.y > localBox.GetCenter().y) {
                    return upperRight->GetColorTransformed(pt, c);
                } else {
                    return lowerRight->GetColorTransformed(pt, c);
                }
            } else {
                if (pt.y > localBox.GetCenter().y) {
                    return upperLeft->GetColorTransformed(pt, c);
                } else {
                    return lowerLeft->GetColorTransformed(pt, c);
                }
            }
        }

        return GetLocalColorTransformed(pt, c);
    }

    bool QuadtreePattern::IsInsideTransformed(const Vector2<double> &pt) const
    {
        if (!localBox.IsInside(pt)) {
            return false;
        }

        if (!leaf) {
            if (pt.x > localBox.GetCenter().x) {
                if (pt.y > localBox.GetCenter().y) {
                    return upperRight->IsInsideTransformed(pt);
                } else {
                    return lowerRight->IsInsideTransformed(pt);
                }
            } else {
                if (pt.y > localBox.GetCenter().y) {
                    return upperLeft->IsInsideTransformed(pt);
                } else {
                    return lowerLeft->IsInsideTransformed(pt);
                }
            }
        }

        for (auto child : children) {
            if (child->IsInside(pt))
                return true;
        }

        return false;
    }

    bool QuadtreePattern::GetLocalColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        bool hasColor = false;
        for (auto pattern : children) {
            Color4d localColor;
            if (pattern->GetColor(pt, localColor)) {
                if (hasColor) {
                    c.Blend(localColor);
                } else {
                    hasColor = true;
                    c = localColor;
                }
            }
        }

        return hasColor;
    }

    std::unique_ptr<QuadtreePattern> QuadtreePattern::CreateNode(double dx, double dy)
    {
        auto result = std::make_unique<QuadtreePattern>(maxElems, maxDepth, debug);
        result->leaf = true;
        result->depth = depth + 1;
        result->localBox.SetWidth(localBox.GetHRadius());
        result->localBox.SetHeight(localBox.GetVRadius());
        result->localBox.SetCenter(localBox.GetCenter() + Vector2<double>(dx, dy));
        result->scan = scan;
        result->scanFactor = scanFactor;
        //result->scanFactor = scanFactor * pow(2, result->depth);
        return result;
    }

    void QuadtreePattern::SetDebug(bool b)
    {
        debug = b;
    }

    bool QuadtreePattern::IsDebug() const
    {
        return debug;
    }

    bool QuadtreePattern::IsInsideScan(Pattern &pattern) const
    {
        auto bbox = pattern.GetBoundingBox();
        if (!localBox.IsInside(bbox))
            return false;

        double dx = localBox.GetWidth() * scanFactor;
        double dy = localBox.GetHeight() * scanFactor;

        Vector2d p;
        for (double y = localBox.GetLowerBound() + dy / 2; y < localBox.GetUpperBound(); y += dy) {
            for (double x = localBox.GetLeftBound() + dx / 2; x < localBox.GetRightBound(); x+= dx) {
                if (pattern.IsInside(Vector2(x, y)))
                    return true;
            }
        }

        return false;
    }

} // end of namespace