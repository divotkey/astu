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
#include "VertexBuffer2D.h"
#include "Controllable.h"
#include "Transform2.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "Color.h"

namespace astu {

    // Forward declaration
    class Polyline2D;
    class Node2D;

    /////////////////////////////////////////////////
    /////// SceneRenderer2D
    /////////////////////////////////////////////////

    class SceneRenderer2D {
    public:

        /** Virtual destructor. */
        virtual ~SceneRenderer2D() {}

        /**
         * Renders a polyine node.
         * 
         * @param polyline  the polyline node to render
         * @param alpha     the transparency
         */
        virtual void Render(Polyline2D& polyline, float alpha) = 0;
    };

    /////////////////////////////////////////////////
    /////// Spatial2D
    /////////////////////////////////////////////////

    class Spatial2D : public Controllable {
    public:

        /** Virtual destructor. */
        virtual ~Spatial2D() {}

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
         * @param _name  then name
         */
        void SetName(const std::string & _name) {
            name = _name;
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
         * Retrieves the local transformation matrix  of this spatial.
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
         * @param alpha     the transparancy
         */
        virtual void Render(SceneRenderer2D& renderer, float alpha) = 0;

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
        Node2D* GetParent() const {
            return parent;
        }

        /**
         * Sets the transparance of this spatial.
         * 
         * @param alpha the transparance within the range [0, 1]
         */
        void SetTransparance(float alpha);

        /**
         * Returns the transparency of this spatial.
         * 
         * @return the transparency within the range [0, 1]
         */
        float GetTransparency() const {
            return alpha;
        }

        /**
         * Creats a copy of this spatial.
         * 
         * @return the copy
         */
        virtual std::shared_ptr<Spatial2D> Clone() const = 0;

    protected:
        /** The parent of this spatial. */
        Node2D* parent;

        /** The name of this spatial. */
        std::string name;

        /** The transparancy. */
        float alpha;

        /**
         * Constructor. 
         */ 
        Spatial2D();

        /**
         * Copy constructor.
         * 
         * @param o the other spatial from which to copy
         */
        Spatial2D(const Spatial2D &o);

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
         * @param newParent the parent
         */
        void SetParent(Node2D *newParent) {
            parent = newParent;
        }

    private:
        /** The local affine transformation of this spatial. */
        Transform2f localTransform;

        /** The world transformation matrix of this spatial. */
        Matrix3f worldMatrix;

        /** The local transformation matrix of this spatial. */
        Matrix3f localMatrix;

        friend class Node2D;
    };

    /////////////////////////////////////////////////
    /////// Node2D
    /////////////////////////////////////////////////

    class Node2D : public Spatial2D {
    public:

        /**
         * Tests whether the specified spatial has alreaedy been added.
         * 
         * @return child    the spatial to test
         * @return `true` if the spatial has already been added as child
         */
        bool HasChild(std::shared_ptr<Spatial2D> child);

        /**
         * Attaches a child to this node.
         * 
         * The specified child must neither be child of any other node, 
         * nor a child of this node, other wise the behaviour is undefied and
         * might result in an exception.
         * 
         * @param child the child to attach
         */
        void AttachChild(std::shared_ptr<Spatial2D> child);

        /**
         * Detaches a child from this node.
         * 
         * The specified child node must be a child of this node otherwise
         * the behaviour is undefined and might result in an exception.
         * 
         * @param child the child to detach
         */
        void DetachChild(std::shared_ptr<Spatial2D> child);

        /**
         * Removes a child with the specified name.
         * 
         * @param childName the name of the child to remove
         * @return  `true` if a child with the specified name has been found
         *          and removed
         */
        bool DetachChild(const std::string & childName) {
            auto child = FindChildOrNull(name);
            if (child && child->HasParent()) {
                child->GetParent()->DetachChild(child);
                return true;
            }
            return false;
        }

        /**
         * Searches recursively for a child node with a specific name.
         * 
         * @param childName the name of the child node to search for
         * @return  the requested child or `nullptr` if no child with the 
         *          specified name has been found
         */
        std::shared_ptr<Spatial2D> FindChildOrNull(const std::string & childName);

        /**
         * Searches recursively for a child node with a specific name.
         * 
         * @param childName the name of the child node to search for
         * @return  the requested child
         * @throws  std::logic_error in case no child with the specified name
         *          has been found
         */
        std::shared_ptr<Spatial2D> FindChild(const std::string & childName);

        // Inherited via Spatial2
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial2D> Clone() const override;

    protected:
        // Inherited via Spatial2
        virtual void UpdateTransform(double dt) override;

    private:
        /** The children of this node. */
        std::vector<std::shared_ptr<Spatial2D>> children;
    };

    // A possible intermediate class if leaf nodes, that share things.
    // class Geometry2 {
    // public:

    //     /** Virtual destructor. */
    //     virtual ~Geometry2() {}

    // private:
    // };

    /////////////////////////////////////////////////
    /////// Polyline2D
    /////////////////////////////////////////////////

    class Polyline2D final : public Spatial2D {
    public:

        /**
         * Constructor.
         * 
         * @param vertexBuffer  the vertex data
         */
        Polyline2D(std::shared_ptr<VertexBuffer2D> vertexBuffer);

        void SetColor(const Color4f& c);

        const Color4f& GetColor() const {
            return color;
        }

        /**
         * Returns the vertex buffer of this polyline.
         * 
         * @return the vertex buffer
         */
        VertexBuffer2D& GetVertexBuffer() {
            return *vertexBuffer;
        }

        // Inherited via Node2/Spatial2
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial2D> Clone() const override;

    private:
        /** The vertex buffer representing the vertex data of this polyline. */
        std::shared_ptr<VertexBuffer2D> vertexBuffer;

        /** The color of this polyline. */
        Color4f color;
    };

    /////////////////////////////////////////////////
    /////// SceneGraph2D
    /////////////////////////////////////////////////

    /**
     * Represents a scene graph in two-dimensional space.
     */
    class SceneGraph2D {
    public:

        /** The default name of the root node. */
        static const std::string DEFAULT_ROOT_NAME;

        /**
         * Constructor.
         */
        SceneGraph2D();

        /** Virtual destructor. */
        virtual ~SceneGraph2D() {}

        /**
         * Returns the root node of this scene graph.
         * 
         * @return the root node
         */
        std::shared_ptr<Node2D> GetRoot() {
            return root;
        }

        /**
         * Returns the root node of this scene graph.
         * 
         * @return the root node
         */
        std::shared_ptr<const Node2D> GetRoot() const {
            return root;
        }

    private:
        /** The root node of this scene graph. */
        std::shared_ptr<Node2D> root;
    };

    /////////////////////////////////////////////////
    /////// SpatialBuilder2D
    /////////////////////////////////////////////////

    template <typename T>
    class SpatialBuilder2D {
    public:

        /** Virtual destructor. */
        virtual ~SpatialBuilder2D() {}

        T& Name(const std::string &spatialName) {
            name = spatialName;
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

        void Build(Spatial2D& spatial) {
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
    /////// NodeBuilder2D
    /////////////////////////////////////////////////

    class NodeBuilder2D final : public SpatialBuilder2D<NodeBuilder2D> {
    public:

        /**
         * Constructor.
         */
        NodeBuilder2D() {
            Reset();
        }

        NodeBuilder2D&  AttachChild(std::shared_ptr<Spatial2D> child) {
            children.push_back(child);
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        NodeBuilder2D& Reset() {
            SpatialBuilder2D::Reset();
            return *this;
        }

        /**
         * Creates a new polyline according to the current configuration.
         * 
         * @return the newly created polyline
         */
        std::shared_ptr<Node2D> Build() {
            auto result = std::make_shared<Node2D>();
            SpatialBuilder2D::Build(*result);
            for (auto child : children) {
                result->AttachChild(child);
            }
            children.clear();

            return result;
        }

    private:
        /** The children of this node. */
        std::vector<std::shared_ptr<Spatial2D>> children;
    };

    /////////////////////////////////////////////////
    /////// Polyline2Builder
    /////////////////////////////////////////////////

    class PolylineBuilder2D final : public SpatialBuilder2D<PolylineBuilder2D> {
    public:

        /**
         * Constructor.
         */
        PolylineBuilder2D() {
            Reset();
        }

        PolylineBuilder2D& Color(const Color4f c) {
            color = c;
            return *this;
        }

        /**
         * Specifies the vertex buffer to be used.
         * 
         * @param vb    the vertex buffer
         * @return reference to this builder for method chaining
         */
        PolylineBuilder2D& VertexBuffer(std::shared_ptr<VertexBuffer2D> vb) {
            vertexBuffer = vb;
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        PolylineBuilder2D& Reset() {
            SpatialBuilder2D::Reset();
            vertexBuffer = nullptr;
            color = WebColors::Aqua;
            return *this;
        }

        /**
         * Creates a new polyline according to the current configuration.
         * 
         * @return the newly created polyline
         */
        std::shared_ptr<Polyline2D> Build() {
            if (!vertexBuffer) {
                throw std::logic_error(
                    "Unable to build Polyline2, vertex buffer not specified");
            }
            auto result = std::make_shared<Polyline2D>(vertexBuffer);
            SpatialBuilder2D::Build(*result);
            result->SetColor(color);

            return result;
        }

    private:
        /** The vertex buffer used to build the polyline. */
        std::shared_ptr<VertexBuffer2D> vertexBuffer;    

        /** The color of the polyline to build. */
        Color4f color;

    };
    
} // end of namespace