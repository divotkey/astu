/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/PolygonPattern2.h"
#include "Graphics/TrianglePattern.h"
#include "Graphics/UnicolorPattern.h"
#include "Graphics/UnionPattern.h"
#include "Graphics/IntersectionPattern.h"
#include "Graphics/LinePattern.h"
#include "Graphics/CirclePattern.h"
#include "Math/Line2.h"
#include "Math/Random.h"

// C++ Standard Library includes
#include <algorithm>
#include <stdexcept>
#include <string>

using namespace std;

namespace astu {

    PolygonPattern2::PolygonPattern2()
        : outlineWidth(0.01), quadtree(make_shared<QuadtreePattern>(5, 9)), debug(false)
    {
        // Intentionally left empty.
    }

    bool PolygonPattern2::GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const
    {
        return delegatePattern->GetColor(pt, outColor);
    }

    BoundingBox PolygonPattern2::GetLocalBoundingBox() const
    {
        return delegatePattern->GetBoundingBox();
    }

    void PolygonPattern2::AddVertex(const Vector2d &vtx)
    {
        vertices.push_back(vtx);
    }

    bool PolygonPattern2::IsInsideTransformed(const Vector2<double> &pt) const
    {
        return delegatePattern->IsInside(pt);
    }

    void PolygonPattern2::Prepare()
    {
        if (vertices.size() < 3) {
            return;
        }

        // TODO determine winding order of polygon.

        //cout << "optimizing polygon with " << vertices.size() << endl;

        // Create copy of list of vertices.
        vector<Vector2d> lst = vertices;
        if (IsClockwise()) {
            //cout << "polygon is clockwise" << endl;
            reverse(lst.begin(), lst.end());
        }

        // As long as there are more than three vertices left, an "ear" that can be cut off, must exist.
        while (lst.size() > 3) {

            // Find ear.
            bool foundEar = false;
            for (size_t i = 1; i < lst.size(); ++i) {
                //cout << lst.size() << "  vertices left " << endl;
                Vector2 p1 = lst[i - 1];
                Vector2 p = lst[i];
                Vector2 p2 = i + 1 == lst.size() ? lst[0] : lst[i + 1];

                if (IsEar(lst, p1, p, p2)) {
                    quadtree->AddPattern(CreateTriangle(p1, p, p2));
                    lst.erase(lst.begin() + i);
                    foundEar = true;
                }
            }

            if (!foundEar) {
                cout << "fuck, polygon2 is broken" << endl;
                break;
            }

        }

        if (lst.size() == 3) {
            quadtree->AddPattern(CreateTriangle(lst[0], lst[1], lst[2]));
        }
        quadtree->BuildTree();

        if (outlinePattern) {
            auto compound = make_shared<UnionPattern>();
            compound->AddPattern(quadtree);
            auto outline = make_shared<IntersectionPattern>();
            outline->AddPattern(CreateOutline());
            outline->AddPattern(quadtree);
            compound->AddPattern(outline);
            delegatePattern = compound;
        } else {
            delegatePattern = quadtree;
        }

        // This will call GetLocalBoundingBox, so it should be the last call in this method.
        Pattern::Prepare();
    }

    bool PolygonPattern2::IsEar(const vector<Vector2d> &lst, const Vector2d &p1, const Vector2d &p, const Vector2d &p2) const
    {
        double l = ((p1.x - p.x) * (p2.y - p.y) - (p1.y - p.y) * (p2.x - p.x));

        if (l >= 0) {
            return false;
        }

        for (const auto &x : lst) {

            if (x == p1 || x == p || x == p2) {
                continue;
            }

            // p0, p1, p2, p
            if (IsInsideTriangle(p1, p, p2, x))
                return false;
        }

        return true;
    }

    bool PolygonPattern2::IsInsideTriangle(const Vector2d &p0, const Vector2d &p1, const Vector2d &p2,
                                           const Vector2d &pt) const
    {
        double s = (p0.x - p2.x) * (pt.y - p2.y) - (p0.y - p2.y) * (pt.x - p2.x);
        double t = (p1.x - p0.x) * (pt.y - p0.y) - (p1.y - p0.y) * (pt.x - p0.x);

        if ((s < 0) != (t < 0) && s != 0 && t != 0)
            return false;

        double d = (p2.x - p1.x) * (pt.y - p1.y) - (p2.y - p1.y) * (pt.x - p1.x);
        return d == 0 || (d < 0) == (s + t <= 0);
    }

    void PolygonPattern2::SetDebug(bool b)
    {
        debug = b;
    }

    bool PolygonPattern2::IsDebug() const
    {
        return debug;
    }

    void PolygonPattern2::SetDebugQuadtree(bool b)
    {
        quadtree->SetDebug(b);
    }

    bool PolygonPattern2::IsDebugQuadtree() const
    {
        return quadtree->IsDebug();
    }

    bool PolygonPattern2::IsClockwise() const
    {
        int idx = static_cast<int>(ChooseConvexHullVertex());

        const Vector2d &a = vertices[MathUtils::GetIndex(idx - 1, vertices.size())];
        const Vector2d &b = vertices[idx];
        const Vector2d &c = vertices[MathUtils::GetIndex(idx +1, vertices.size())];


        double d = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);

        return d < 0;
    }

    size_t PolygonPattern2::ChooseConvexHullVertex() const
    {
        assert(!vertices.empty());

        vector<size_t> tmp;

        // collect vertices with smallest x-coordinates.
        double value = vertices.front().x;
        tmp.push_back(0);

        for (size_t i = 1; i < vertices.size(); ++i) {
            const auto &vertex = vertices[i];

            if (value > vertex.x) {
                value = vertex.x;
                tmp.clear();
                tmp.push_back(i);
            } else if (value == vertex.x) {
                tmp.push_back(i);
            }
        }

        assert(tmp.size() > 0);

        // Only one vertex has the smallest x-coordinate.
        if (tmp.size() == 1)
            return tmp[0];

        // Choose the vertex with the greatest y-coordinate.
        value = vertices[tmp[0]].y;
        size_t result = tmp[0];

        for (size_t i = 1; i < tmp.size(); ++i) {
            const auto &vertex = vertices[tmp[i]];

            if (value < vertex.y) {
                value = vertex.y;
                result = tmp[i];
            }
        }

        return result;
    }

    shared_ptr<TrianglePattern> PolygonPattern2::CreateTriangle(const Vector2d &p1, const Vector2d &p2, const Vector2d &p3)
    {
        auto triangle = make_shared<TrianglePattern>(p1, p2, p3);
        if (debug) {
            triangle->SetPattern(UnicolorPattern::CreateRandomColored());
        } else {
            triangle->SetPattern(GetPattern());
        }

        return triangle;
    }

    std::shared_ptr<Pattern> PolygonPattern2::CreateOutline()
    {
        assert(vertices.size() >= 3);

        auto outline = make_shared<QuadtreePattern>();

        auto p0 = vertices[vertices.size() - 1];

        auto circle = make_shared<CirclePattern>(outlineWidth / 2.0);
        circle->SetPattern(outlinePattern);
        circle->Translate(p0);
        outline->AddPattern(circle);

        for (size_t i = 0; i < vertices.size(); ++i) {
            auto p1 = vertices[i];
            auto line = make_shared<LinePattern>();
            line->SetPoint1(p0);
            line->SetPoint2(p1);
            line->SetPattern(outlinePattern);
            line->SetWidth(outlineWidth);
            outline->AddPattern(line);

            circle = make_shared<CirclePattern>(outlineWidth / 2.0);
            circle->SetPattern(outlinePattern);
            circle->Translate(p1);
            outline->AddPattern(circle);
            p0 = p1;
        }

        outline->BuildTree();
        return outline;
    }


} // end of namespace