/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Service.h"
#include "Graphics/VertexBuffer2.h"
#include "Graphics/Texture.h"
#include "Graphics/Font.h"
#include "Util/Controllable.h"
#include "Graphics/WebColors.h"
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
    class Sprite;
    class TextSprite;
    class Node;

    /////////////////////////////////////////////////
    /////// SceneRenderer2D
    /////////////////////////////////////////////////

    /**
     * Base class for scene renderers.
     * 
     * Scene renderers are used to render leaf elements of the scene graph. 
     * 
     * @ingroup suite2d_group
     */
    class SceneRenderer2D {
    public:

        /** Virtual destructor. */
        virtual ~SceneRenderer2D() {}

        /**
         * Renders a polyline node.
         * 
         * @param polyline  the polyline node to render
         * @param alpha     the transparency
         */
        virtual void Render(Polyline& polyline, float alpha) = 0;

        /**
         * Renders a sprite node.
         *
         * @param sprite    the sprite node to render
         * @param alpha     the transparency
         */
        virtual void Render(Sprite &sprite, float alpha) = 0;

        ///**
        // * Renders a sprite node.
        // *
        // * @param textSprite    the text sprite node to render
        // * @param alpha         the transparency
        // */
        //virtual void Render(TextSprite &textSprite, float alpha) = 0;
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
         * Retrieves the local transformation matrix of this spatial.
         * 
         * @return the local transformation
         */
        Transform2f& GetLocalTransform() {
            return  localTransform;
        }

        /**
         * Retrieves the local transformation matrix of this spatial.
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
         * Sets the transparency of this spatial.
         * 
         * @param alpha the transparency within the range [0, 1]
         */
        void SetTransparency(float alpha);

        /**
         * Returns the transparency of this spatial.
         * 
         * @return the transparency within the range [0, 1]
         */
        float GetTransparency() const {
            return alpha;
        }

        /**
         * Whether the visibility flag of this spatial is is set to true.
         *
         * @return `true` if this spatial is visible
         */
        bool IsVisible() const {
            return visible;
        }

        /**
         * Sets the visibility flag to the specified value.
         *
         * @param b the boolean state of the visibility flag
         */
        void SetVisible(bool b) {
            visible = b;
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

        /** The transparency. */
        float alpha;

        /** Whether this spacial is visible. */
        bool visible;

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
         * Tests whether the specified spatial has already been added.
         * 
         * @return child    the spatial to test
         * @return `true` if the spatial has already been added as child
         */
        bool HasChild(std::shared_ptr<Spatial> child);

        /**
         * Attaches a child to this node.
         * 
         * The specified child must neither be child of any other node, 
         * nor a child of this node, other wise the behaviour is undefined and
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

        // Inherited via Spatial
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial> Clone() const override;

    protected:
        // Inherited via Spatial
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
     * It consists of contiguous lines that do not need to be closed.  
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
         * Returns whether the polyline is rendered as closed shape.
         * 
         * @return `true` if the polyline is rendered as closed shape
         */
        bool IsClosed() const {
            return closed;
        }

        /**
         * Defines whether the polyline is renderer as line or as closed shape.
         * 
         * @param b set to `true` to render the polyline as closed shape
         */
        void SetClosed(bool b);

        /**
         * Returns the vertex buffer of this polyline.
         * 
         * @return the vertex buffer
         */
        VertexBuffer2f& GetVertexBuffer() {
            return *vertexBuffer;
        }

        // Inherited via Node2/Spatial
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial> Clone() const override;

    private:
        /** The vertex buffer representing the vertex data of this polyline. */
        std::shared_ptr<VertexBuffer2f> vertexBuffer;

        /** Whether the polyline should be rendered as closed shape. */
        bool closed;

        /** The color of this polyline. */
        Color4f color;
    };

    /**
     * The sprite class represents a leaf element within the scene graph.
     * It consists of a texture as well as size information in world space.
     *
     * @ingroup suite2d_group
     */
    class Sprite : public Spatial {
    public:

        /**
         * Constructor.
         *
         * @param texture   the texture used by this sprite
         */
        Sprite(std::shared_ptr<Texture> texture);

        /**
         * Constructor.
         *
         * @param texture   the texture used by this sprite
         * @parma width     the width of this sprite in world space
         * @parma height    the height of this sprite in world space
         */
        Sprite(std::shared_ptr<Texture> texture, float width, float height);

        /**
         * Returns the width of this sprite in world space.
         *
         * @return the width of this sprite in the game world
         */
        float GetWidth() const {
            return width;
        }

        /**
         * Sets the width of this sprite in world space.
         *
         * @param w width in world space
         */
        void SetWidth(float w);

        /**
         * Returns the height of this sprite in world space.
         *
         * @return the height of this sprite in the game world
         */
        float GetHeight() const {
            return height;
        }

        /**
         * Sets the height of this sprite in world space.
         *
         * @param h height in world space
         */
        void SetHeight(float h);

        /**
         * Returns the texture used by this sprite.
         *
         * @return the sprite's texture
         */
        Texture &GetTexture();

        /**
         * Returns the texture used by this sprite.
         *
         * @return the sprite's texture
         */
        const Texture &GetTexture() const;

        // Inherited via Node2/Spatial
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial> Clone() const override;

    protected:

        /**
         * Constructor.
         */
        Sprite();

        /** The texture representing the visual representation of this sprite. */
        std::shared_ptr<Texture> texture;

    private:
        /** The width of this sprite in world space. */
        float width;

        /** The height of this sprite in world space. */
        float height;
    };

    /**
     * The text sprite class represents a leaf element within the scene graph.
     * It consists of a font used to generate textures when the text ist changed.
     *
     * @ingroup suite2d_group
     */
    class TextSprite final : public Sprite {
    public:

        /**
         * Constructor.
         *
         * @param font  the font used by this sprite
         */
        TextSprite(std::shared_ptr<Font> font, const std::string &text);

        void SetText(const std::string &text);
        const std::string& GetText() const;
        std::shared_ptr<Font> GetFont() const;

        /**
         * Sets the color of this text sprite.
         *
         * @param color the color
         */
        void SetColor(const Color4f& color);

        /**
         * Returns the color of text sprite.
         *
         * @return this text sprite's color
         */
        const Color4f& GetColor() const {
            return color;
        }

        // Inherited via Node2/Spatial
        virtual void Render(SceneRenderer2D& renderer, float alpha) override;
        virtual std::shared_ptr<Spatial> Clone() const override;

    private:
        /** The texture representing the visual representation of this sprite. */
        std::shared_ptr<Font> font;

        /** The text represented by this sprite. */
        std::string text;

        /** The color of this text sprite. */
        Color4f color;

        /** Indicates that the texture needs to be recreated. */
        bool dirty;
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
         * Specifies the state of the visibility flag of the scene graph element to build.
         *
         * @param b the visibility flag
         * @return reference to this builder for method chaining
         */
        T& Visible(bool b) {
            visible = b;
            return reinterpret_cast<T&>(*this);
        }

        /**
         * Sets the transparency of the scene graph element to build.
         *
         * @param a the transparency within the range [0, 1]
         * @return reference to this builder for method chaining
         */
        T& Transparency(float a) {
            if (a < 0 || a > 1) {
                throw std::logic_error("Transparency must be within the range [0, 1], got " + std::to_string(a));
            }
            alpha = a;

            return reinterpret_cast<T&>(*this);
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        T& Reset() {
            visible = true;
            localTransform.SetIdentity();
            alpha = 1.0;
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
            spatial.SetVisible(visible);
            spatial.SetTransparency(alpha);
        }

    private:
        /** The local transformation of the spatial to build. */
        Transform2f localTransform;

        /** The name of the spatial to build. */
        std::string name;

        /** The visibility flag of the spatial to build. */
        bool visible;

        /** Defines the transparency of the spatial to build. */
        float alpha;
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
         * Creates a new node according to the current configuration.
         *      
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
         * Specifies the vertices used to create the polyline.
         * 
         * @param vtx   the vertices
         * @return reference to this builder for method chaining
         */
        PolylineBuilder& VertexBuffer(const std::vector<astu::Vector2f>& vtx) {
            return VertexBuffer(
                ASTU_SERVICE(VertexBufferBuilder2f)
                    .Reset().AddVertices(vtx).Build()
            );
        }

        /**
         * Specifies whether the polyline is rendered as closed shape.
         * 
         * @param b set to `true` to render the polyline closed
         * @return reference to this builder for method chaining
         */
        PolylineBuilder& Closed(bool b) {
            closed = b;
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
            closed = true;
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
            result->SetClosed(closed);

            return result;
        }

    private:
        /** The vertex buffer used to build the polyline. */
        std::shared_ptr<VertexBuffer2f> vertexBuffer;    

        /** Whether to polyline is closed. */
        bool closed;

        /** The color of the polyline to build. */
        Color4f color;

    };

    /////////////////////////////////////////////////
    /////// SpriteBuilder
    /////////////////////////////////////////////////

    /**
     * This fluent builder is used to build new sprite scene graph elements.
     *
     * @ingroup suite2d_group
     */
    class SpriteBuilder final : public SpatialBuilder<SpriteBuilder> {
    public:

        /**
         * Constructor.
         */
        SpriteBuilder() {
            Reset();
        }

        /**
         * Specifies the texture used to create the sprite.
         *
         * @param image the image data used to create the texture for the sprite
         * @param sq    the scale quality for the texture to create
         * @return reference to this builder for method chaining
         */
        SpriteBuilder& Tex(Image &image, TextureFactory::ScaleQuality sq = TextureFactory::ScaleQuality::Nearest) {
            texture = ASTU_SERVICE(TextureFactory).CreateFromImage(image, sq);
            return *this;
        }

        /**
         * Specifies the texture used to create the sprite.
         *
         * @param tex   the sprite texture
         * @return reference to this builder for method chaining
         */
        SpriteBuilder& Tex(std::shared_ptr<Texture> tex) {
            texture = tex;
            return *this;
        }

        /**
         * Specifies the size of the sprite in world space.
         *
         * @param w the width in world space
         * @param h the height in world space
         * @return reference to this builder for method chaining
         */
        SpriteBuilder& Size(float w, float h) {
            width = w;
            height = h;
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         *
         * @return reference to this builder for method chaining
         */
        SpriteBuilder& Reset() {
            SpatialBuilder::Reset();
            texture = nullptr;
            width = height = 0.0f;
            return *this;
        }

        /**
         * Creates a new sprite according to the current configuration.
         *
         * @return the newly created polyline
         */
        std::shared_ptr<Sprite> Build() {
            if (!texture) {
                throw std::logic_error(
                        "Unable to build Sprite, texture specified");
            }

            float w = width > 0 ? width : texture->GetWidth();
            float h = height > 0 ? height : texture->GetHeight();


            auto result = std::make_shared<Sprite>(texture, w, h);
            SpatialBuilder::Build(*result);

            return result;
        }

    private:
        /** The width of the sprite in world space. */
        float width;

        /** The height of the sprite in world space. */
        float height;

        /** The texture used for the sprite. */
        std::shared_ptr<Texture> texture;
    };


    /////////////////////////////////////////////////
    /////// TextSpriteBuilder
    /////////////////////////////////////////////////

    /**
     * This fluent builder is used to build new text sprite scene graph elements.
     *
     * @ingroup suite2d_group
     */
    class TextSpriteBuilder final : public SpatialBuilder<TextSpriteBuilder> {
    public:

        /**
         * Constructor.
         */
        TextSpriteBuilder() {
            Reset();
        }

        /**
         * Specifies the texture used to create the sprite.
         *
         * @param font the image data used to create the texture for the sprite
         * @return reference to this builder for method chaining
         */
        TextSpriteBuilder& FontRef(std::shared_ptr<Font> font) {
            this->font = font;
            return *this;
        }

        /**
         * Specifies the texture used to create the sprite.
         *
         * @param text  the text the sprite should display
         * @return reference to this builder for method chaining
         */
        TextSpriteBuilder& Text(const std::string &text) {
            this->text = text;
            return *this;
        }

        /**
         * Specifies the size of the sprite in world space.
         *
         * @param color the color of the text
         * @return reference to this builder for method chaining
         */
        TextSpriteBuilder& Color(const Color4f &color) {
            this->color = color;
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         *
         * @return reference to this builder for method chaining
         */
        TextSpriteBuilder& Reset() {
            SpatialBuilder::Reset();
            font = nullptr;
            text = "TextSprite";
            color = WebColors::Cyan;
            return *this;
        }

        /**
         * Creates a new sprite according to the current configuration.
         *
         * @return the newly created polyline
         */
        std::shared_ptr<Sprite> Build() {
            if (!font) {
                throw std::logic_error(
                        "Unable to build Sprite, texture specified");
            }


            auto result = std::make_shared<TextSprite>(font, text);
            result->SetColor(color);
            SpatialBuilder::Build(*result);

            return result;
        }

    private:
        /** The texture used for the sprite. */
        std::shared_ptr<Font> font;

        std::string text;

        Color4f color;
    };

} // end of namespace