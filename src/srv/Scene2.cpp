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

    /////////////////////////////////////////////////
    /////// Node2
    /////////////////////////////////////////////////

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

    void Node2::Render(Scene2Renderer& renderer)
    {
        for (auto child : children) {
            child->Render(renderer);
        }
    }

    /////////////////////////////////////////////////
    /////// Polyline2
    /////////////////////////////////////////////////

    void Polyline2::Render(Scene2Renderer& renderer)
    {
        renderer.Render(*this);
    }

} // end of namespace