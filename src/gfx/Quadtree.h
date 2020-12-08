/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include <array>

#include "Pattern.h"

namespace astu {

    class Quadtree : public CompoundPattern {
    public:

        /**
         * Constructor.
         * 
         * @param maxElems  the maximum number of elements in one onde
         * @param maxDepth  theh maximum depth of this tree
         */
        Quadtree(int maxElems = 5, int maxDepth = 5);

        /**
         * Builds the quad tree.
         */
        void BuildTree();

    protected:

        // Inherited via CompoundPattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;
        virtual void OnPatternAdded(Pattern & pattern) override;
        virtual void OnClear() override;

    private:
        /** The upper left child node of this node. */
        std::unique_ptr<Quadtree> upperLeft;

        /** The upper right child node of this node. */
        std::unique_ptr<Quadtree> upperRight;

        /** The lower left child node of this node. */
        std::unique_ptr<Quadtree> lowerLeft;

        /** The lower right child node of this node. */
        std::unique_ptr<Quadtree> lowerRight;

        /** The maximum number fo elements alowed for one single node. */
        int maxElems;

        /** The maximum depth of this tree. */
        int maxDepth;

        /** The depth level of this node. */
        int depth;

        /** Determines whether this node is a leaf node. */
        bool leaf;

        /** The bounding box of this octree in local space. */
        BoundingBox localBox;

        std::unique_ptr<Quadtree> CreateNode(double dx, double dy);
        bool GetLocalColorTransformed(const Vector2<double> &pt, Color & c) const;

    };

}