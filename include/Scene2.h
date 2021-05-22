/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

// Local includes
#include "VertexBuffer2.h"
#include "Controllable.h"
#include "Transform2.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "Color.h"

namespace astu {

    // Forward declaration
    class Polyline2;

    /////////////////////////////////////////////////
    /////// Scene2Renderer
    /////////////////////////////////////////////////

    class Scene2Renderer {
    public:

        /** Virtual destructor. */
        virtual ~Scene2Renderer() {}

        /**
         * Renders a polyine node.
         * 
         * @param polyline  the polyline node to render
         */
        virtual void Render(Polyline2& polyline) = 0;
    };

    /////////////////////////////////////////////////
    /////// Spatial2
    /////////////////////////////////////////////////

    class Spatial2 : public Controllable {
    public:

        /** Virtual destructor. */
        virtual ~Spatial2() {}

        /**
         * Returns the name of this spatial.
         * 
         * @return name;
         */
        const std::string& GetName() const {
            return name;
        }

        /**
         * Sets the name of this spatial.
         * 
         * @param name  then name
         */
        void SetName(const std::string & name) {
            this->name = name;
        }

        /**
         * Sets the local transformation of this spatial.
         * 
         * @param tx    the transformation 
         */
        void SetLocalTransform(const Transform2f & tx) {
            localTransform = tx;
        }

        /**
         * Retrieves the local transormation matrix  of this spatial.
         * 
         * @return the local transformation
         */
        Transform2f& GetLocalTransform() {
            return  localTransform;
        }

        /**
         * Retrieves the local transormation matrix  of this spatial.
         * 
         * @return the local transformation
         */
        const Transform2f& GetLocalTransform() const {
            return  localTransform;
        }

        const Matrix3f& GetWorldMatrix() const {
            return worldMatrix;
        }

        /**
         * Updates the geometric state.
         * 
         * This method updates attached controllers, computes world
         * transformations  etc.
         * 
         * @param dt    the elapsed time since the last update in seconds
         */
        void Update(double dt);

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

    protected:
        /** The parent of this spatial. */
        Spatial2* parent;

        /** The name of this spatial. */
        std::string name;

        /**
         * Constructor. 
         */ 
        Spatial2();

        /**
         * Updates the world transformation of this spatial.
         * 
         * @param dt    the elapsed time since the last update in seconds
         */
        virtual void UpdateTransform(double dt);

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

    private:
        /** The local affine transformation of this spatial. */
        Transform2f localTransform;

        /** The world transformation matrixof this spatial. */
        Matrix3f worldMatrix;

        /** The local transformation matrix of this spatial. */
        Matrix3f localMatrix;

        friend class Node2;
    };

    /////////////////////////////////////////////////
    /////// Node2
    /////////////////////////////////////////////////

    class Node2 : public Spatial2 {
    public:

        /**
         * Tests whether the specified spatial has alreaedy been added.
         * 
         * @return child    the spatial to test
         * @return `true` if the spatial has already been added as child
         */
        bool HasChild(std::shared_ptr<Spatial2> child);

        void AttachChild(std::shared_ptr<Spatial2> child);
        void DetachChild(std::shared_ptr<Spatial2> child);

        std::shared_ptr<Spatial2> FindChildOrNull(const std::string & name);

        std::shared_ptr<Spatial2> FindChild(const std::string & name);

        // Inherited via Spatial2
        virtual void Render(Scene2Renderer& renderer) override;

    protected:
        // Inherited via Spatial2
        virtual void UpdateTransform(double dt) override;

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

    /////////////////////////////////////////////////
    /////// Polyline2
    /////////////////////////////////////////////////

    class Polyline2 final : public Spatial2 {
    public:

        /**
         * Constructor.
         * 
         * @param vertexBuffer  the vertex data
         */
        Polyline2(std::shared_ptr<VertexBuffer2> vertexBuffer);

        void SetColor(const Color4f& c);

        const Color4f& GetColor() const {
            return color;
        }

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
        /** The vertex buffer representing the vertex data of this polyline. */
        std::shared_ptr<VertexBuffer2> vertexBuffer;

        /** The color of this polyline. */
        Color4f color;
    };

    /////////////////////////////////////////////////
    /////// SceneGraph2
    /////////////////////////////////////////////////

    /**
     * Represents a scene graph in two-dimensional space.
     */
    class SceneGraph2 {
    public:

        /** The default name of the root node. */
        static const std::string DEFAULT_ROOT_NAME;

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
        std::shared_ptr<Node2> GetRoot() {
            return root;
        }

        /**
         * Returns the root node of this scene graph.
         * 
         * @return the root node
         */
        std::shared_ptr<const Node2> GetRoot() const {
            return root;
        }

    private:
        /** The root node of this scene graph. */
        std::shared_ptr<Node2> root;
    };

    /////////////////////////////////////////////////
    /////// Spatial2Builder
    /////////////////////////////////////////////////

    template <typename T>
    class Spatial2Builder {
    public:

        /** Virtual destructor. */
        virtual ~Spatial2Builder() {}

        T& Name(const std::string &name) {
            this->name = name;
            return reinterpret_cast<T&>(*this);
        }

        T& Translation(const Vector2f& v) {
            localTransform.SetTranslation(v);
            return reinterpret_cast<T&>(*this);
        }

        T& Translation(float x, float y) {
            localTransform.SetTranslation(x, y);
            return reinterpret_cast<T&>(*this);
        }

        T& Scaling(float s) {
            localTransform.SetScaling(s, s);
            return reinterpret_cast<T&>(*this);
        }

        T& Scaling(const Vector2f& s) {
            localTransform.SetScaling(s);
            return reinterpret_cast<T&>(*this);
        }

        T& Scaling(float sx, float sy) {
            localTransform.SetScaling(sx, sy);
            return reinterpret_cast<T&>(*this);
        }

        T& Rotation(float phi) {
            localTransform.SetRotation(phi);
            return reinterpret_cast<T&>(*this);
        }

        T& RotationDeg(float phi) {
            localTransform.SetRotationDeg(phi);
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        T& Reset() {
            localTransform.SetIdentity();
            return reinterpret_cast<T&>(*this);
        }

    protected:

        void Build(Spatial2& spatial) {
            spatial.SetLocalTransform(localTransform);
            spatial.SetName(name);
        }

    private:
        /** The local transformation of the spatial to build. */
        Transform2f localTransform;

        /** The name of the spatial to build. */
        std::string name;
    };

    /////////////////////////////////////////////////
    /////// Node2Builder
    /////////////////////////////////////////////////

    class Node2Builder final : public Spatial2Builder<Node2Builder> {
    public:

        /**
         * Constructor.
         */
        Node2Builder() {
            Reset();
        }

        Node2Builder&  AttachChild(std::shared_ptr<Spatial2> child) {
            children.push_back(child);
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        Node2Builder& Reset() {
            Spatial2Builder::Reset();
            return *this;
        }

        /**
         * Creates a new polyline according to the current configuration.
         * 
         * @return the newly created polyline
         */
        std::shared_ptr<Node2> Build() {
            auto result = std::make_shared<Node2>();
            Spatial2Builder::Build(*result);
            for (auto child : children) {
                result->AttachChild(child);
            }
            children.clear();

            return result;
        }

    private:
        /** The children of this node. */
        std::vector<std::shared_ptr<Spatial2>> children;
    };

    /////////////////////////////////////////////////
    /////// Polyline2Builder
    /////////////////////////////////////////////////

    class Polyline2Builder final : public Spatial2Builder<Polyline2Builder> {
    public:

        /**
         * Constructor.
         */
        Polyline2Builder() {
            Reset();
        }

        Polyline2Builder& Color(const Color4f c) {
            color = c;
            return *this;
        }

        /**
         * Specifies the vertex buffer to be used.
         * 
         * @param vb    the vertex buffer
         * @return reference to this builder for method chaining
         */
        Polyline2Builder& VertexBuffer(std::shared_ptr<VertexBuffer2> vb) {
            vertexBuffer = vb;
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        Polyline2Builder& Reset() {
            Spatial2Builder::Reset();
            vertexBuffer = nullptr;
            color = WebColors::Aqua;
            return *this;
        }

        /**
         * Creates a new polyline according to the current configuration.
         * 
         * @return the newly created polyline
         */
        std::shared_ptr<Polyline2> Build() {
            if (!vertexBuffer) {
                throw std::logic_error(
                    "Unable to build Polyline2, vertex buffer not specified");
            }
            auto result = std::make_shared<Polyline2>(vertexBuffer);
            Spatial2Builder::Build(*result);
            result->SetColor(color);

            return result;
        }

    private:
        /** The vertex buffer used to build the polyline. */
        std::shared_ptr<VertexBuffer2> vertexBuffer;    

        /** The color of the polyline to build. */
        Color4f color;

    };
} // end of namespace