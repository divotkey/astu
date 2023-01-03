/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

#include <memory>
#include <array>

#include "Graphics/CompoundPattern.h"

namespace astu {

    class QuadtreePattern : public CompoundPattern {
    public:

        /**
         * Constructor.
         * 
         * @param maxElems  the maximum number of elements in one node
         * @param maxDepth  the maximum depth of this tree
         */
        QuadtreePattern(int maxElems = 5, int maxDepth = 5);

        /**
         * Builds the quad tree.
         */
        void BuildTree();

    protected:

        // Inherited via CompoundPattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;
        virtual void OnPatternAdded(Pattern & pattern) override;
        virtual void OnClear() override;

    private:
        /** The upper left child node of this node. */
        std::unique_ptr<QuadtreePattern> upperLeft;

        /** The upper right child node of this node. */
        std::unique_ptr<QuadtreePattern> upperRight;

        /** The lower left child node of this node. */
        std::unique_ptr<QuadtreePattern> lowerLeft;

        /** The lower right child node of this node. */
        std::unique_ptr<QuadtreePattern> lowerRight;

        /** The maximum number fo elements allowed for one single node. */
        int maxElems;

        /** The maximum depth of this tree. */
        int maxDepth;

        /** The depth level of this node. */
        int depth;

        /** Determines whether this node is a leaf node. */
        bool leaf;

        /** The bounding box of this octree in local space. */
        BoundingBox localBox;

        std::unique_ptr<QuadtreePattern> CreateNode(double dx, double dy);
        bool GetLocalColorTransformed(const Vector2<double> &pt, Color4d & c) const;

    };

}