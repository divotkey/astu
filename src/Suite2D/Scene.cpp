/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Suite2D/Scene.h"

// C++ Standard Library includes
#include <algorithm>
#include <cassert>

using namespace std;

namespace astu::suite2d {

    /////////////////////////////////////////////////
    /////// SceneGraph
    /////////////////////////////////////////////////

    const std::string SceneGraph::DEFAULT_ROOT_NAME = "ROOT";

    SceneGraph::SceneGraph()
        : root(NodeBuilder().Name(DEFAULT_ROOT_NAME).Build())
    {
        // Intentionally left empty.
    }

    /////////////////////////////////////////////////
    /////// Spatial
    /////////////////////////////////////////////////

    Spatial::Spatial()
        : parent(nullptr)
        , alpha(1.0f)
    {
        // Intentionally left empty.
    }

    Spatial::Spatial(const Spatial &o)
        : parent(nullptr)
        , name(o.name)
        , alpha(o.alpha)
        , localTransform(o.localTransform)
        , worldMatrix(o.worldMatrix)
        , localMatrix(o.localMatrix)
    {
        // Intentionally left empty.
    }

    void Spatial::SetTransparency(float inAlpha)
    {
        assert(alpha >= 0.0f && alpha <= 1.0f);
        alpha = alpha;
    }

    void Spatial::Update(double dt)
    {
        UpdateTransform(dt);
    }

    void Spatial::UpdateTransform(double dt)
    {
        UpdateControllers(dt);

        if (parent) {
            worldMatrix = parent->worldMatrix * localTransform.StoreToMatrix(localMatrix);
        } else {
            worldMatrix = localTransform.StoreToMatrix(localMatrix);
        }
    }

    /////////////////////////////////////////////////
    /////// Node
    /////////////////////////////////////////////////

    std::shared_ptr<Spatial> Node::FindChildOrNull(const std::string & childName)
    {
        for(auto & child : children) {
            if (child->GetName() == childName) {
                return child;
            }
        }

        for(auto & child : children) {
            auto node = std::dynamic_pointer_cast<Node>(child);
            if (node) {
                auto result = node->FindChild(childName);
                if (result) {
                    return result;
                }
            }
        }

        return nullptr;
    }

    std::shared_ptr<Spatial> Node::FindChild(const std::string & childName)
    {
        auto result = FindChildOrNull(childName);
        if (!result) {
            throw std::logic_error(
                "No spatial with name '" + childName + "' found");
        }

        return result;
    }


    bool Node::HasChild(std::shared_ptr<Spatial> child)
    {
        return find(children.begin(), children.end(), child) != children.end();
    }

    void Node::AttachChild(std::shared_ptr<Spatial> child)
    {
        assert(!child->HasParent());
        assert(!HasChild(child));

        child->SetParent(this);
        children.push_back(child);
    }

    void Node::DetachChild(std::shared_ptr<Spatial> child)
    {
        assert(HasChild(child));
        assert(child->GetParent() == this);

        children.erase(
            std::remove(children.begin(), children.end(), child), 
            children.end());

        child->SetParent(nullptr);
    }

    void Node::DetachAll()
    {
        for (auto & child : children) {
            child->SetParent(nullptr);
        }
        children.clear();
    }

    void Node::UpdateTransform(double dt)
    {
        Spatial::UpdateTransform(dt);

        for (auto & child : children) {
            child->UpdateTransform(dt);
        }
    }

    void Node::Render(SceneRenderer2D& renderer, float alpha)
    {
        for (auto child : children) {
            child->Render(renderer, alpha * child->GetTransparency());
        }
    }

    std::shared_ptr<Spatial> Node::Clone() const
    {
        auto result = make_shared<Node>();

        for (const auto & child : children) {
            result->AttachChild( child->Clone() );
        }
        return result;
    }

    /////////////////////////////////////////////////
    /////// Polyline
    /////////////////////////////////////////////////

    Polyline::Polyline(std::shared_ptr<VertexBuffer2f> vertexBuffer)
        : vertexBuffer(vertexBuffer)
    {
        // Intentionally left empty
    }

    void Polyline::Render(SceneRenderer2D& renderer, float alpha)
    {
        renderer.Render(*this, alpha);
    }

    void Polyline::SetColor(const Color4f& c)
    {
        color = c;
    }

    void Polyline::SetClosed(bool b)
    {
        closed = b;
    }

    std::shared_ptr<Spatial> Polyline::Clone() const
    {
        return std::make_shared<Polyline>(*this);
    }

    /////////////////////////////////////////////////
    /////// Sprite
    /////////////////////////////////////////////////

    Sprite::Sprite(std::shared_ptr<Texture> texture)
        : texture(texture), width(1.0), height(1.0)
    {
        if (!texture) {
            throw std::logic_error("Unable to create sprite, specified texture must not be null");
        }
        width = static_cast<float>(texture->GetWidth());
        height = static_cast<float>(texture->GetHeight());
    }

    Sprite::Sprite(std::shared_ptr<Texture> texture, float width, float height)
        : texture(texture), width(width), height(height)
    {
        assert(texture);
    }


    Texture &Sprite::GetTexture()
    {
        return *texture;
    }

    const Texture &Sprite::GetTexture() const
    {
        return *texture;
    }

    void Sprite::Render(SceneRenderer2D &renderer, float alpha)
    {
        renderer.Render(*this, alpha);
    }

    std::shared_ptr<Spatial> Sprite::Clone() const
    {
        return std::make_shared<Sprite>(*this);
    }

    void Sprite::SetWidth(float w)
    {
        assert(w >= 0);
        width = w;
    }

    void Sprite::SetHeight(float h)
    {
        assert(h >= 0);
        height = h;
    }

} // end of namespace