/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Scene2.h"

// C++ Standard Library includes
#include <algorithm>
#include <cassert>

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// SceneGraph2
    /////////////////////////////////////////////////

    const std::string SceneGraph2::DEFAULT_ROOT_NAME = "ROOT";

    SceneGraph2::SceneGraph2()
        : root(Node2Builder().Name(DEFAULT_ROOT_NAME).Build())
    {
        // Intentionally left empty.
    }

    /////////////////////////////////////////////////
    /////// Spatial2
    /////////////////////////////////////////////////

    Spatial2::Spatial2()
        : parent(nullptr)
        , alpha(1.0f)
    {
        // Intentionally left empty.
    }

    Spatial2::Spatial2(const Spatial2 &o)
        : parent(nullptr)
        , name(o.name)
        , alpha(o.alpha)
        , localTransform(o.localTransform)
        , worldMatrix(o.worldMatrix)
        , localMatrix(o.localMatrix)
    {
        // Intentionally left empty.
    }

    void Spatial2::SetTransparance(float inAlpha)
    {
        assert(alpha >= 0.0f && alpha <= 1.0f);
        alpha = inAlpha;
    }

    void Spatial2::Update(double dt)
    {
        UpdateTransform(dt);
    }

    void Spatial2::UpdateTransform(double dt)
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

    std::shared_ptr<Spatial2> Node2::FindChildOrNull(const std::string & childName)
    {
        for(auto & child : children) {
            if (child->GetName() == childName) {
                return child;
            }
        }

        for(auto & child : children) {
            auto node = std::dynamic_pointer_cast<Node2>(child);
            if (node) {
                auto result = node->FindChild(childName);
                if (result) {
                    return result;
                }
            }
        }

        return nullptr;
    }

    std::shared_ptr<Spatial2> Node2::FindChild(const std::string & childName)
    {
        auto result = FindChildOrNull(childName);
        if (!result) {
            throw std::logic_error("No spatial with name '" + childName + "' found");
        }

        return result;
    }


    bool Node2::HasChild(std::shared_ptr<Spatial2> child)
    {
        return find(children.begin(), children.end(), child) != children.end();
    }

    void Node2::AttachChild(std::shared_ptr<Spatial2> child)
    {
        assert(!child->HasParent());
        assert(!HasChild(child));

        child->SetParent(this);
        children.push_back(child);
    }

    void Node2::DetachChild(std::shared_ptr<Spatial2> child)
    {
        assert(HasChild(child));
        assert(child->GetParent() == this);

        children.erase(
            std::remove(children.begin(), children.end(), child), 
            children.end());

        child->SetParent(nullptr);
    }

    void Node2::UpdateTransform(double dt)
    {
        Spatial2::UpdateTransform(dt);

        for (auto & child : children) {
            child->UpdateTransform(dt);
        }
    }

    void Node2::Render(Scene2Renderer& renderer, float alpha)
    {
        for (auto child : children) {
            child->Render(renderer, alpha * child->GetTransparency());
        }
    }

    std::shared_ptr<Spatial2> Node2::Clone() const
    {
        auto result = make_shared<Node2>();

        for (const auto & child : children) {
            result->AttachChild( child->Clone() );
        }
        return result;
    }

    /////////////////////////////////////////////////
    /////// Polyline2
    /////////////////////////////////////////////////

    Polyline2::Polyline2(std::shared_ptr<VertexBuffer2> vertexBuffer)
        : vertexBuffer(vertexBuffer)
    {
        // Intentionally left empty
    }

    void Polyline2::Render(Scene2Renderer& renderer, float alpha)
    {
        renderer.Render(*this, alpha);
    }

    void Polyline2::SetColor(const Color4f& c)
    {
        color = c;
    }

    std::shared_ptr<Spatial2> Polyline2::Clone() const
    {
        return std::make_shared<Polyline2>(*this);
    }

} // end of namespace