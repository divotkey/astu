/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <iostream>
#include "Quadtree.h"

using namespace std;

namespace astu {

    Quadtree::Quadtree(int _maxElems, int _maxDepth)
        : maxElems(_maxElems)
        , maxDepth(_maxDepth)
        , depth(0)
        , leaf(false)
    {
        // Intentionally left empty.
    }

    void printBox(const BoundingBox & box, const char *col)
    {
        cout << "box(<" << box.GetCenter().x << ", " << box.GetCenter().y 
        << ">, " << box.GetWidth() << ", " << box.GetHeight() << ", " << col << ");" << endl;
    }

    void Quadtree::BuildTree()
    {
        if (children.size() < maxElems || depth >= maxDepth) {
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
                upperLeft->Add(child);
                // cout << "upper left" << endl;
                // printBox(upperLeft->localBox, "BLUE");
                // printBox(box, "RED");
            }

            if (upperRight->localBox.IsInside(box)) {
                upperRight->Add(child);
                // cout << "upper right" << endl;
                // printBox(upperRight->localBox, "GREEN");
                // printBox(box, "RED");
            }

            if (lowerLeft->localBox.IsInside(box)) {
                lowerLeft->Add(child);
                // cout << "lower left" << endl;
                // printBox(lowerLeft->localBox, "YELLOW");
                // printBox(box, "RED");
            }

            if (lowerRight->localBox.IsInside(box)) {
                lowerRight->Add(child);
                // cout << "lower right" << endl;
                // printBox(lowerRight->localBox, "ORANGE");
                // printBox(box, "RED");
            }
        }

        upperLeft->BuildTree();
        upperRight->BuildTree();
        lowerLeft->BuildTree();
        lowerRight->BuildTree();
        children.clear();
    }

    void Quadtree::OnPatternAdded(Pattern & pattern)
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

    void Quadtree::OnClear()
    {
        localBox.Reset();
        upperLeft.reset();
        upperRight.reset();
        lowerLeft.reset();
        lowerRight.reset();
    }

    BoundingBox Quadtree::GetLocalBoundingBox() const
    {
        return localBox;
    }

    bool Quadtree::GetColorTransformed(const Vector2<double> &pt, Color & c) const
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

    bool Quadtree::GetLocalColorTransformed(const Vector2<double> &pt, Color & c) const
    {
        bool hasColor = false;
        for (auto pattern : children) {
            Color localColor;
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

    std::unique_ptr<Quadtree> Quadtree::CreateNode(double dx, double dy)
    {
        auto result = std::make_unique<Quadtree>(maxElems, maxDepth);
        result->leaf = true;
        result->depth = depth + 1;
        result->localBox.SetWidth(localBox.GetHRadius());
        result->localBox.SetHeight(localBox.GetVRadius());
        result->localBox.MoveCenter(dx, dy);
        result->localBox.SetCenter(localBox.GetCenter() + Vector2<double>(dx, dy));
        return result;
    }

}