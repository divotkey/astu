/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/QuadtreePattern.h"

// C++ Standard Library includes
#include <cassert>
#include <iostream>
#include <string>

using namespace std;

namespace astu {

    QuadtreePattern::QuadtreePattern(int inMaxElems, int inMaxDepth)
        : depth(0)
        , leaf(false)
    {
        SetMaxElems(inMaxElems);
        SetMaxDepth(inMaxDepth);
    }

    void QuadtreePattern::SetMaxElems(int n)
    {
        if (n < 0) {
            throw std::domain_error("Maximum number of elements for quadtree mast be greater zero, got " + to_string(n));
        }

        maxElems = n;
    }

    void QuadtreePattern::SetMaxDepth(int n)
    {
        if (n < 0) {
            throw std::domain_error("Maximum depth of quadtree mast be greater zero, got " + to_string(n));
        }

        maxDepth = n;
    }

    void printBox(const BoundingBox & box, const char *col)
    {
        cout << "box(<" << box.GetCenter().x << ", " << box.GetCenter().y 
        << ">, " << box.GetWidth() << ", " << box.GetHeight() << ", " << col << ");" << endl;
    }

    void QuadtreePattern::BuildTree()
    {
        if (static_cast<int>(children.size()) < maxElems || depth >= maxDepth) {
            leaf = true;
            return;
        }
        leaf = false;

        upperLeft = CreateNode(-localBox.GetHRadius() / 2, localBox.GetVRadius() / 2);
        upperRight = CreateNode(localBox.GetHRadius() / 2, localBox.GetVRadius() / 2);
        lowerLeft = CreateNode(-localBox.GetHRadius() / 2, -localBox.GetVRadius() / 2);
        lowerRight = CreateNode(localBox.GetHRadius() / 2, -localBox.GetVRadius() / 2);

        // printBox(upperLeft->localBox, "BLUE");
        // printBox(upperRight->localBox, "GREEN");
        // printBox(lowerLeft->localBox, "YELLOW");
        // printBox(lowerRight->localBox, "ORANGE");


        for (auto child : children) {
            const auto & box = child->GetBoundingBox();

            if (upperLeft->localBox.IsInside(box)) {
                upperLeft->AddPattern(child);
                // cout << "upper left" << endl;
                // printBox(upperLeft->localBox, "BLUE");
                // printBox(box, "RED");
            }

            if (upperRight->localBox.IsInside(box)) {
                upperRight->AddPattern(child);
                // cout << "upper right" << endl;
                // printBox(upperRight->localBox, "GREEN");
                // printBox(box, "RED");
            }

            if (lowerLeft->localBox.IsInside(box)) {
                lowerLeft->AddPattern(child);
                // cout << "lower left" << endl;
                // printBox(lowerLeft->localBox, "YELLOW");
                // printBox(box, "RED");
            }

            if (lowerRight->localBox.IsInside(box)) {
                lowerRight->AddPattern(child);
                // cout << "lower right" << endl;
                // printBox(lowerRight->localBox, "ORANGE");
                // printBox(box, "RED");
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

        if (localBox.IsZero() && !box.IsInfinite()) {
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

            return false;
        }


        return GetLocalColorTransformed(pt, c);
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
        auto result = std::make_unique<QuadtreePattern>(maxElems, maxDepth);
        result->leaf = true;
        result->depth = depth + 1;
        result->localBox.SetWidth(localBox.GetHRadius());
        result->localBox.SetHeight(localBox.GetVRadius());
        result->localBox.MoveCenter(dx, dy);
        result->localBox.SetCenter(localBox.GetCenter() + Vector2<double>(dx, dy));
        return result;
    }

} // end of namespace