/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <vector>
#include <memory>

namespace astu {

    // Forward declaration
    class VertexBuffer2;
    class Polyline2;

    class Scene2Renderer {
    public:

        /** Virtual destructor. */
        virtual ~Scene2Renderer() {}

        virtual void Render(Polyline2& polyline) = 0;
    };

    class Spatial2 {
    public:

        /** Virtual destructor. */
        virtual ~Spatial2() {}

        /**
         * Renders this spacial.
         * 
         * @param renderer  the renderer to be used
         */
        virtual void Render(Scene2Renderer& renderer) = 0;

        /**
         * Returns whether this spatial has a parent.
         * 
         * @return `true` if this spatial has a parent
         */
        bool HasParent() const {
            return parent != nullptr;
        }

        /**
         * Returns the parent of this spatial.
         * 
         * @return the parent or nullptr if this patial has not parrent
         */
        Spatial2* GetParent() const {
            return parent;
        }

        /**
         * Sets the parent of this spatial.
         * 
         * Do not call this method directly.
         * 
         * @param parent    the parent
         */
        void SetParent(Spatial2 *parent) {
            this->parent = parent;
        }

    protected:
        /** The parent of this spatial. */
        Spatial2* parent;

        /**
         * Constructor. 
         */ 
        Spatial2();
    };

    class Node2 : public Spatial2 {
    public:

        bool HasChild(std::shared_ptr<Spatial2> child);
        void AttachChild(std::shared_ptr<Spatial2> child);
        void DetachChild(std::shared_ptr<Spatial2> child);

        // Inherited via Spatial2
        virtual void Render(Scene2Renderer& renderer) override;

    private:
        /** The children of this node. */
        std::vector<std::shared_ptr<Spatial2>> children;
    };

    // A possible intermediate class if leaf nodes, that share things.
    // class Geometry2 {
    // public:

    //     /** Virtual destructor. */
    //     virtual ~Geometry2() {}

    // private:
    // };

    class Polyline2 final : public Node2 {
    public:

        /**
         * Returns the vertex buffer of this polyline.
         * 
         * @return the vertex buffer
         */
        VertexBuffer2& GetVertexBuffer() {
            return *vertexBuffer;
        }

        // Inherited via Node2/Spatial2
        virtual void Render(Scene2Renderer& renderer) override;

    private:
        std::shared_ptr<VertexBuffer2> vertexBuffer;
    };


    /**
     * Represents a scene graph in two-dimensional space.
     */
    class SceneGraph2 {
    public:

        /**
         * Constructor.
         */
        SceneGraph2();

        /** Virtual destructor. */
        virtual ~SceneGraph2() {}

        /**
         * Returns the root node of this scene graph.
         * 
         * @return the root node
         */
        Node2& GetRoot() {
            return *root;
        }

        /**
         * Returns the root node of this scene graph.
         * 
         * @return the root node
         */
        const Node2& GetRoot() const {
            return *root;
        }

    private:
        /** The root node of this scene graph. */
        std::shared_ptr<Node2> root;
    };

} // end of namespace