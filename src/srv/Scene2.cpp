/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <algorithm>
#include <cassert>

// Local includes
#include "Scene2.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// SceneGraph2
    /////////////////////////////////////////////////

    SceneGraph2::SceneGraph2()
        : root(std::make_shared<Node2>())
    {
        // Intentionally left empty.
    }

    /////////////////////////////////////////////////
    /////// Spatial2
    /////////////////////////////////////////////////

    Spatial2::Spatial2()
        : parent(nullptr)
    {
        // Intentionally left empty.
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

    std::shared_ptr<Spatial2> Node2::FindChildOrNull(const std::string & name)
    {
        for(auto & child : children) {
            if (child->GetName() == name) {
                return child;
            }
        }

        for(auto & child : children) {
            auto node = std::dynamic_pointer_cast<Node2>(child);
            if (node) {
                auto result = node->FindChild(name);
                if (result) {
                    return result;
                }
            }
        }

        return nullptr;
    }

    std::shared_ptr<Spatial2> Node2::FindChild(const std::string & name)
    {
        auto result = FindChildOrNull(name);
        if (!result) {
            throw std::logic_error("No spatial with name '" + name + "' found");
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

    void Node2::Render(Scene2Renderer& renderer)
    {
        for (auto child : children) {
            child->Render(renderer);
        }
    }

    /////////////////////////////////////////////////
    /////// Polyline2
    /////////////////////////////////////////////////

    Polyline2::Polyline2(std::shared_ptr<VertexBuffer2> vertexBuffer)
        : vertexBuffer(vertexBuffer)
    {
        // Intentionally left empty
    }

    void Polyline2::Render(Scene2Renderer& renderer)
    {
        renderer.Render(*this);
    }

    void Polyline2::SetColor(const Color4f& c)
    {
        color = c;
    }

} // end of namespace