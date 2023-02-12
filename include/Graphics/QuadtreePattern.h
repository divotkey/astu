/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include <memory>
#include <array>

#include "Graphics/CompoundPattern.h"
#include "Graphics/RectanglePattern.h"

namespace astu {

    class QuadtreePattern : public CompoundPattern {
    public:

        /**
         * Constructor.
         * 
         * @param maxElems  the maximum number of elements in one node
         * @param maxDepth  the maximum depth of this tree
         * @param debug     whether to enable debug visualization
         */
        QuadtreePattern(int maxElems = 5, int maxDepth = 5, bool debug = false);

        /**
         * Specifies the maximum number of elements per node.
         *
         * @param n the maximum number of elements
         */
        void SetMaxElems(int n);

        /**
         * Returns the maximum number of elements per node.
         *
         * @return the maximum number of elements
         */
        int GetMaxElems() const {
            return  maxElems;
        }

        /**
         * Specifies the maximum recursion depth of this quadtree.
         *
         * @param n the maximum recursion depth
         */
        void SetMaxDepth(int n);

        /**
         * Returns the maximum recursion depth of this quadtree.
         *
         * @return the maximum recursion depth
         */
        int GetMaxDepth() const {
            return maxDepth;
        }

        /**
         * Enables or disables debug visualization.
         *
         * @param b
         */
        void SetDebug(bool b);

        /**
         * Returns whether debug visualization is enabled or disabled.
         *
         * @return `true` if debug visualization is enabled
         */
        bool IsDebug() const;

        /**
         * Builds the quad tree.
         */
        void BuildTree();

        void SetScan(bool b) {
            scan = b;
        }

        void SetScanFactor(double p) {
            scanFactor = p;
        }

    protected:

        // Inherited via CompoundPattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;
        virtual void OnPatternAdded(Pattern & pattern) override;
        virtual void OnClear() override;

        bool IsInsideTransformed(const Vector2<double> &pt) const override;

    private:
        /** The upper left child node of this node. */
        std::unique_ptr<QuadtreePattern> upperLeft;

        /** The upper right child node of this node. */
        std::unique_ptr<QuadtreePattern> upperRight;

        /** The lower left child node of this node. */
        std::unique_ptr<QuadtreePattern> lowerLeft;

        /** The lower right child node of this node. */
        std::unique_ptr<QuadtreePattern> lowerRight;

        /* Whether debug visualization is enabled or disabled. */
        bool debug;

        std::shared_ptr<RectanglePattern> debugPattern;

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

        bool scan;
        double scanFactor;

        std::unique_ptr<QuadtreePattern> CreateNode(double dx, double dy);
        bool GetLocalColorTransformed(const Vector2<double> &pt, Color4d & c) const;

        bool IsInsideScan(Pattern &pattern) const;
    };

} // end of namespace