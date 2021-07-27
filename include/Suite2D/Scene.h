/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Util/Controllable.h"
#include "Graphics/WebColors.h"
#include "Graphics/VertexBuffer2.h"
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
#include "Math/Transform2.h"
#include "Graphics/Color.h"

// C++ Standard Library includes
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

namespace astu::suite2d {

    // Forward declaration
    class Polyline;
    class Node;

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
        virtual void Render(Polyline& polyline, float alpha) = 0;
    };

    /////////////////////////////////////////////////
    /////// Spatial
    /////////////////////////////////////////////////

    /**
     * A Spatial represents the basic scene graph element.
     * The main property of a Spatial is that it contains a transformation, 
     * that describes the position, orientation ans scaling of this scene
     * graph element.
     * 
     * @ingroup suite2d_group
     */
    class Spatial : public Controllable {
    public:

        /** Virtual destructor. */
        virtual ~Spatial() {}

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

        /**
         * Returns the world transformation matrix of this spatial.
         * The world transformation matrix is only valid after the Update()
         * method has been called.
         * 
         * @return the world transformation matrix
         */
        const Matrix3f& GetWorldMatrix() const {
            return worldMatrix;
        }

        /**
         * Updates the geometric state.
         * 
         * This method updates attached controllers, computes world
         * transformations etc.
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
        Node* GetParent() const {
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
        virtual std::shared_ptr<Spatial> Clone() const = 0;

    protected:
        /** The parent of this spatial. */
        Node* parent;

        /** The name of this spatial. */
        std::string name;

        /** The transparancy. */
        float alpha;

        /**
         * Constructor. 
         */ 
        Spatial();

        /**
         * Copy constructor.
         * 
         * @param o the other spatial from which to copy
         */
        Spatial(const Spatial &o);

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
        void SetParent(Node *newParent) {
            parent = newParent;
        }

    private:
        /** The local affine transformation of this spatial. */
        Transform2f localTransform;

        /** The world transformation matrix of this spatial. */
        Matrix3f worldMatrix;

        /** The local transformation matrix of this spatial. */
        Matrix3f localMatrix;

        friend class Node;
    };

    /////////////////////////////////////////////////
    /////// Node
    /////////////////////////////////////////////////

    /**
     * A node is a spatial scene graph element that manages children.
     * 
     * @ingroup suite2d_group
     */
    class Node : public Spatial {
    public:

        /**
         * Tests whether the specified spatial has alreaedy been added.
         * 
         * @return child    the spatial to test
         * @return `true` if the spatial has already been added as child
         */
        bool HasChild(std::shared_ptr<Spatial> child);

        /**
         * Attaches a child to this node.
         * 
         * The specified child must neither be child of any other node, 
         * nor a child of this node, other wise the behaviour is undefied and
         * might result in an exception.
         * 
         * @param child the child to attach
         */
        void AttachChild(std::shared_ptr<Spatial> child);

        /**
         * Detaches a child from this node.
         * 
         * The specified child node must be a child of this node otherwise
         * the behaviour is undefined and might result in an exception.
         * 
         * @param child the child to detach
         */
        void DetachChild(std::shared_ptr<Spatial> child);

        /**
         * Removes a child with the specified name.
         * 
         * @param childName the name of the child to remove
         * @return  `true` if a child with the specified name has been found
         *          and removed
         */
        bool DetachChild(const std::string & childName) {
            auto child = FindChildOrNull(childName);
            if (child && child->HasParent()) {
                child->GetParent()->DetachChild(child);
                return true;
            }
            return false;
        }

        /**
         * Removes all attached child nodes.
         */
        void DetachAll();

        /**
         * Searches recursively for a child node with a specific name.
         * 
         * @param childName the name of the child node to search for
         * @return  the requested child or `nullptr` if no child with the 
         *          specified name has been found
         */
        std::shared_ptr<Spatial> FindChildOrNull(const std::string & childName);

        /**
         * Searches recursively for a child node with a specific name.
         * 
         * @param childName the name of the child node to search for
         * @return  the requested child
         * @throws  std::logic_error in case no child with the specified name
         *          has been found
         */
        std::shared_ptr<Spatial> FindChild(const std::string & childName);

        // Inherited via Spatial2
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial> Clone() const override;

    protected:
        // Inherited via Spatial2
        virtual void UpdateTransform(double dt) override;

    private:
        /** The children of this node. */
        std::vector<std::shared_ptr<Spatial>> children;
    };

    // A possible intermediate class if leaf nodes, that share things.
    // class Geometry {
    // public:

    //     /** Virtual destructor. */
    //     virtual ~Geometry() {}

    // private:
    // };

    /////////////////////////////////////////////////
    /////// Polyline
    /////////////////////////////////////////////////

    /**
     * The polyline class represents a leaf element within the scene graph.
     * It consists of contiguous lines that do not need to be closed. Thus, a
     * polyline is not a polygon in which the last vertex is always connected
     * to the first vertex.
     * 
     * If a polygon is to be represented using a polyline element, the first
     * vertex must be duplicated in order to achieve a closed representation. 
     * 
     * @ingroup suite2d_group
     */
    class Polyline final : public Spatial {
    public:

        /**
         * Constructor.
         * 
         * @param vertexBuffer  the vertex data
         */
        Polyline(std::shared_ptr<VertexBuffer2f> vertexBuffer);

        /**
         * Sets the color of this polyline.
         * 
         * @param color the color
         */
        void SetColor(const Color4f& color);

        /**
         * Returns the color of this polyline.
         * 
         * @return this polyline's color
         */
        const Color4f& GetColor() const {
            return color;
        }

        /**
         * Returns the vertex buffer of this polyline.
         * 
         * @return the vertex buffer
         */
        VertexBuffer2f& GetVertexBuffer() {
            return *vertexBuffer;
        }

        // Inherited via Node2/Spatial2
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial> Clone() const override;

    private:
        /** The vertex buffer representing the vertex data of this polyline. */
        std::shared_ptr<VertexBuffer2f> vertexBuffer;

        /** The color of this polyline. */
        Color4f color;
    };

    /////////////////////////////////////////////////
    /////// SceneGraph
    /////////////////////////////////////////////////

    /**
     * Represents a scene graph in two-dimensional space.
     * 
     * @ingroup suite2d_group
     */
    class SceneGraph {
    public:

        /** The default name of the root node. */
        static const std::string DEFAULT_ROOT_NAME;

        /**
         * Constructor.
         */
        SceneGraph();

        /** Virtual destructor. */
        virtual ~SceneGraph() {}

        /**
         * Returns the root node of this scene graph.
         * 
         * @return the root node
         */
        std::shared_ptr<Node> GetRoot() {
            return root;
        }

        /**
         * Returns the root node of this scene graph.
         * 
         * @return the root node
         */
        std::shared_ptr<const Node> GetRoot() const {
            return root;
        }

    private:
        /** The root node of this scene graph. */
        std::shared_ptr<Node> root;
    };

    /////////////////////////////////////////////////
    /////// SpatialBuilder
    /////////////////////////////////////////////////

    /**
     * Base class for builders that create scene graph elements.
     * 
     * @ingroup suite2d_group
     */
    template <typename T>
    class SpatialBuilder {
    public:

        /** Virtual destructor. */
        virtual ~SpatialBuilder() {}

        /**
         * Specifies the name of the scene graph element to build.
         * 
         * @param inName    the name of the scene graph element
         * @return reference to this builder for method chaining
         */
        T& Name(const std::string &inName) {
            name = inName;
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Specifies the translation of the scene graph element to build.
         * 
         * @param v the translation vector
         * @return reference to this builder for method chaining
         */
        T& Translation(const Vector2f& v) {
            localTransform.SetTranslation(v);
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Specifies the translation of the scene graph element to build.
         * 
         * @param vx    the x-coordinate of the translation vector
         * @param vy    the y-coordinate of the translation vector
         * @return reference to this builder for method chaining
         */
        T& Translation(float vx, float vy) {
            localTransform.SetTranslation(vx, vy);
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Specifies the scaling of the scene graph element to build.
         * 
         * @param s the scaling factor for x and y dimension
         * @return reference to this builder for method chaining
         */
        T& Scaling(float s) {
            localTransform.SetScaling(s, s);
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Specifies the scaling of the scene graph element to build.
         * 
         * @param vs    the scaling vector
         * @return reference to this builder for method chaining
         */
        T& Scaling(const Vector2f& vs) {
            localTransform.SetScaling(vs);
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Specifies the scaling of the scene graph element to build.
         * 
         * @param sx    the scaling factor for the x-dimension
         * @param sy    the scaling factor for the y-dimension
         * @return reference to this builder for method chaining
         */
        T& Scaling(float sx, float sy) {
            localTransform.SetScaling(sx, sy);
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Specifies the rotation of the scene graph element to build.
         * 
         * @param phi   the rotation angle in radians
         * @return reference to this builder for method chaining
         */
        T& Rotation(float phi) {
            localTransform.SetRotation(phi);
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Specifies the rotation of the scene graph element to build.
         * 
         * @param phi   the rotation angle in degrees
         * @return reference to this builder for method chaining
         */
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

        /**
         * Sets the spatial's parameters according to the current configuration.
         * 
         * @param spatial   the scene graph element
         */
        void Build(Spatial& spatial) {
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
    /////// NodeBuilder
    /////////////////////////////////////////////////

    /**
     * This class is used to build new nodes for the scene graph.
     * 
     * @ingroup suite2d_group
     */
    class NodeBuilder final : public SpatialBuilder<NodeBuilder> {
    public:

        /**
         * Constructor.
         */
        NodeBuilder() {
            Reset();
        }

        /**
         * Adds a new child to be attached to the node to create.
         * 
         * @param child the child
         * @return reference to this builder for method chaining
         */
        NodeBuilder&  AttachChild(std::shared_ptr<Spatial> child) {
            children.push_back(child);
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        NodeBuilder& Reset() {
            SpatialBuilder::Reset();
            return *this;
        }

        /**
         * Creates a new polyline according to the current configuration.
         *      * @ingroup suite2d_group

         * @return the newly created polyline
         */
        std::shared_ptr<Node> Build() {
            auto result = std::make_shared<Node>();
            SpatialBuilder::Build(*result);
            for (auto child : children) {
                result->AttachChild(child);
            }
            children.clear();

            return result;
        }

    private:
        /** The children of this node. */
        std::vector<std::shared_ptr<Spatial>> children;
    };

    /////////////////////////////////////////////////
    /////// PolylineBuilder
    /////////////////////////////////////////////////

    /**
     * This fluent builder is used to build new polyline scene graph elements.
     * 
     * **Example**
     * 
     * ```
     * std::shared_ptr<Node> node = ASTU_SERVICE(SceneGraph).GetRoot();
     *  
     * node->AttachChild(PolylineBuilder()
     *     .Color(RalColors::TrafficWhite)
     *     .Name("Triangle")
     *     .Translation(100, 100)
     *     .RotationDeg(45)
     *     .VertexBuffer(ShapeGenerator().GenTriangle(100))
     *     .Build()
     *   );
     * ```
     * 
     * @ingroup suite2d_group
     */
    class PolylineBuilder final : public SpatialBuilder<PolylineBuilder> {
    public:

        /**
         * Constructor.
         */
        PolylineBuilder() {
            Reset();
        }

        /**
         * Specifies the color used to build the new polyline.
         * 
         * @param c the color
         * @return reference to this builder for method chaining
         */
        PolylineBuilder& Color(const Color4f c) {
            color = c;
            return *this;
        }

        /**
         * Specifies the vertex buffer to be used.
         * 
         * @param vb    the vertex buffer
         * @return reference to this builder for method chaining
         */
        PolylineBuilder& VertexBuffer(std::shared_ptr<VertexBuffer2f> vb) {
            vertexBuffer = vb;
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        PolylineBuilder& Reset() {
            SpatialBuilder::Reset();
            vertexBuffer = nullptr;
            color = WebColors::Aqua;
            return *this;
        }

        /**
         * Creates a new polyline according to the current configuration.
         * 
         * @return the newly created polyline
         */
        std::shared_ptr<Polyline> Build() {
            if (!vertexBuffer) {
                throw std::logic_error(
                    "Unable to build Polyline2, vertex buffer not specified");
            }
            auto result = std::make_shared<Polyline>(vertexBuffer);
            SpatialBuilder::Build(*result);
            result->SetColor(color);

            return result;
        }

    private:
        /** The vertex buffer used to build the polyline. */
        std::shared_ptr<VertexBuffer2f> vertexBuffer;    

        /** The color of the polyline to build. */
        Color4f color;

    };
    
} // end of namespace