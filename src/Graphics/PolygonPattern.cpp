/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/PolygonPattern.h"
#include "Math/Line2.h"

// C++ Standard Library includes
#include <algorithm>
#include <stdexcept>
#include <string>

namespace astu {

    PolygonPattern::PolygonPattern() : outlineWidth(0.01)
    {
        // Intentionally left empty.
    }

    bool PolygonPattern::GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const
    {
        if (pattern && IsInside(pt)) {
            if (outlinePattern &&CalcDistance(pt) < outlineWidth) {
                return outlinePattern->GetColor(pt, outColor);
            }
            return pattern->GetColor(pt, outColor);
        }

        return false;
    }

    BoundingBox PolygonPattern::GetLocalBoundingBox() const
    {
        BoundingBox bbox;

        //for(const auto &vtx : vertices) {
        //    bbox.AddPoint(vtx);
        //}

        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();

        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::lowest();

        for(const auto &vtx : vertices) {
            minX = std::min(minX, vtx.x);
            maxX = std::max(maxX, vtx.x);

            minY = std::min(minY, vtx.y);
            maxY = std::max(maxY, vtx.y);
        }

        bbox.SetWidth(maxX - minX);
        bbox.SetHeight(maxY - minY);
        bbox.SetCenter({minX + bbox.GetWidth() * 0.5, minY + bbox.GetHeight() * 0.5});

        return bbox;
    }

    void PolygonPattern::AddVertex(const Vector2d &vtx)
    {
        vertices.push_back(vtx);
    }

    bool PolygonPattern::IsInsideTransformed(const Vector2<double> &pt) const
    {
        int cnt = 0;

        if (vertices.size() < 3) {
            return false;
        }

        const Vector2d dir = {1.0, 0.0};

        Vector2d last = vertices[vertices.size() - 1];
        for (const auto &vertex : vertices) {
            double s1, s2;
            Line2d line(last, vertex - last);

            if (line.Intersect(last, vertex - last, pt, dir, s1, s2)) {
                if (s1 >= 0 && s1 <= 1 && s2 >= 0) {
                    ++cnt;
                }
            }

            last = vertex;
        }

        return cnt % 2 != 0;
    }

    Vector2d PolygonPattern::CalcGeometricCenter() const
    {
        if (vertices.empty()) {
            return Vector2d::Zero;
        }

        Vector2d center;
        for (const auto &vertex : vertices) {
            center += vertex;
        }

        center /= static_cast<double>(vertices.size());

        return center;
    }

    double PolygonPattern::CalcDistance(const Vector2d &p) const
    {
        // Calculate distance to all edges and take the shortest one.
        double dist = std::numeric_limits<double>::max();

        Vector2d last = vertices[vertices.size() - 1];
        for (const auto &vertex : vertices) {
            Line2d edge(last, vertex - last);
            double s1, s2;
            if (Line2d::Intersect(
                    edge.GetOrigin(),
                    edge.GetDirection(),
                    p,
                    Vector2d(edge.GetDirection()).Perpendicularize().Normalize(),
                    s1, s2) && s1 >= 0.0 && s1 <= 1.0)
            {
                double d = std::abs(s2);
                if (d < dist)
                    dist = d;
            }

            //double d = edge.CalcDistance(p);
            //if (d < dist)
            //    dist = d;

            last = vertex;
        }

        return dist;
    }

} // end of namespace