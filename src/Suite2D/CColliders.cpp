/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Suite2D/CColliders.h"

using namespace std;

namespace astu::suite2d {

    CCircleColliderBuilder::CCircleColliderBuilder(shared_ptr<CCircleColliderFactory> factory)
        : colliderFactory(factory)
    {
        if (!factory) {
            colliderFactory = ASTU_GET_SERVICE(CCircleColliderFactory);
        }
        Reset();
    }

    std::shared_ptr<CCircleCollider> CCircleColliderBuilder::Build()
    {
        auto collider = colliderFactory->CreateCircleCollider();

        Configure(*collider);
        collider->SetRadius(radius);

        return collider;
    }

    CPolygonColliderBuilder::CPolygonColliderBuilder(std::shared_ptr<CPolygonColliderFactory> factory)
        : colliderFactory(factory)
    {
        if (!factory) {
            colliderFactory = ASTU_GET_SERVICE(CPolygonColliderFactory);
        }
        Reset();
    }

    std::shared_ptr<CPolygonCollider> CPolygonColliderBuilder::Build()
    {
        if (!polygon) {
            throw std::logic_error("No polygon specified for polygon collider");
        }
        auto collider = colliderFactory->CreatePolygonCollider();
        Configure(*collider);
        collider->SetPolygon(polygon);

        return collider;

    }

    vector<Vector2f> CPolygonColliderBuilder::tempVertices;

    CPolygonColliderBuilder& CPolygonColliderBuilder::MakeRectangle(float w, float h)
    {
        tempVertices.clear();
        tempVertices.push_back(Vector2f(-w / 2, -h / 2));
        tempVertices.push_back(Vector2f(-w / 2, h / 2));
        tempVertices.push_back(Vector2f(w / 2, h / 2));
        tempVertices.push_back(Vector2f(w / 2, -h / 2));
        polygon = make_shared<Polygon2f>(tempVertices);
        tempVertices.clear();
        return *this;
    }


} // end of namespace