/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Scene2D.h"

// C++ Standard Library includes
#include <algorithm>
#include <cassert>

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// SceneGraph2
    /////////////////////////////////////////////////

    const std::string SceneGraph2D::DEFAULT_ROOT_NAME = "ROOT";

    SceneGraph2D::SceneGraph2D()
        : root(NodeBuilder2D().Name(DEFAULT_ROOT_NAME).Build())
    {
        // Intentionally left empty.
    }

    /////////////////////////////////////////////////
    /////// Spatial2
    /////////////////////////////////////////////////

    Spatial2D::Spatial2D()
        : parent(nullptr)
        , alpha(1.0f)
    {
        // Intentionally left empty.
    }

    Spatial2D::Spatial2D(const Spatial2D &o)
        : parent(nullptr)
        , name(o.name)
        , alpha(o.alpha)
        , localTransform(o.localTransform)
        , worldMatrix(o.worldMatrix)
        , localMatrix(o.localMatrix)
    {
        // Intentionally left empty.
    }

    void Spatial2D::SetTransparance(float inAlpha)
    {
        assert(alpha >= 0.0f && alpha <= 1.0f);
        alpha = inAlpha;
    }

    void Spatial2D::Update(double dt)
    {
        UpdateTransform(dt);
    }

    void Spatial2D::UpdateTransform(double dt)
    {
        UpdateControllers(dt);

        if (parent) {
            worldMatrix = parent->worldMatrix * localTransform.StoreToMatrix(localMatrix);
        } else {
            worldMatrix = localTransform.StoreToMatrix(localMatrix);
        }
    }

    /////////////////////////////////////////////////
    /////// Node2
    /////////////////////////////////////////////////

    std::shared_ptr<Spatial2D> Node2D::FindChildOrNull(const std::string & childName)
    {
        for(auto & child : children) {
            if (child->GetName() == childName) {
                return child;
            }
        }

        for(auto & child : children) {
            auto node = std::dynamic_pointer_cast<Node2D>(child);
            if (node) {
                auto result = node->FindChild(childName);
                if (result) {
                    return result;
                }
            }
        }

        return nullptr;
    }

    std::shared_ptr<Spatial2D> Node2D::FindChild(const std::string & childName)
    {
        auto result = FindChildOrNull(childName);
        if (!result) {
            throw std::logic_error(
                "No spatial with name '" + childName + "' found");
        }

        return result;
    }


    bool Node2D::HasChild(std::shared_ptr<Spatial2D> child)
    {
        return find(children.begin(), children.end(), child) != children.end();
    }

    void Node2D::AttachChild(std::shared_ptr<Spatial2D> child)
    {
        assert(!child->HasParent());
        assert(!HasChild(child));

        child->SetParent(this);
        children.push_back(child);
    }

    void Node2D::DetachChild(std::shared_ptr<Spatial2D> child)
    {
        assert(HasChild(child));
        assert(child->GetParent() == this);

        children.erase(
            std::remove(children.begin(), children.end(), child), 
            children.end());

        child->SetParent(nullptr);
    }

    void Node2D::UpdateTransform(double dt)
    {
        Spatial2D::UpdateTransform(dt);

        for (auto & child : children) {
            child->UpdateTransform(dt);
        }
    }

    void Node2D::Render(SceneRenderer2D& renderer, float alpha)
    {
        for (auto child : children) {
            child->Render(renderer, alpha * child->GetTransparency());
        }
    }

    std::shared_ptr<Spatial2D> Node2D::Clone() const
    {
        auto result = make_shared<Node2D>();

        for (const auto & child : children) {
            result->AttachChild( child->Clone() );
        }
        return result;
    }

    /////////////////////////////////////////////////
    /////// Polyline2
    /////////////////////////////////////////////////

    Polyline2D::Polyline2D(std::shared_ptr<VertexBuffer2D> vertexBuffer)
        : vertexBuffer(vertexBuffer)
    {
        // Intentionally left empty
    }

    void Polyline2D::Render(SceneRenderer2D& renderer, float alpha)
    {
        renderer.Render(*this, alpha);
    }

    void Polyline2D::SetColor(const Color4f& c)
    {
        color = c;
    }

    std::shared_ptr<Spatial2D> Polyline2D::Clone() const
    {
        return std::make_shared<Polyline2D>(*this);
    }

} // end of namespace